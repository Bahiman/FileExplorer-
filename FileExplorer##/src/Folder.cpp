#include "../headers/Folder.hpp" 
#include "../headers/File.hpp"
#include "../utilities/Timer.hpp"
#include "../utilities/CancellableThreadPool.hpp"
#include"../utilities/Other.hpp"
#include <Windows.h>
#include <execution>

uint64_t Folder::get_size(const std::atomic<bool>& p_continue) const
{
	auto contents = get_contents_recursively();

	if (!contents)
		return 0;

	std::atomic<uint64_t> result = 0;

	std::for_each(std::execution::par, contents.value().begin(), contents.value().end(), [&result](auto& ptr) {result += ptr->get_size(); });

	return result;
}

inline std::wstring formatPath(std::wstring_view file_name, std::wstring directory)
{
	if (directory.back() != '\\') // \ 
		directory.append(1, '\\');

	directory.append(file_name);

	return directory;
}

inline void fix_path(std::unique_ptr<FileSystemNode>& elem)
{
	auto& acquired_path = elem->get_modifiable_path();
	std::erase(acquired_path, L'*');
}

void visit_vector(std::variant<std::execution::sequenced_policy, std::execution::parallel_policy>& variant, FSNodes& vector)
{
	std::visit([&](auto policy) {
		std::for_each(policy, vector.begin(), vector.end(), fix_path);
	}, variant);
}


void move_into_vector(FSNodes& target, FSNodes& input)
{
	target.insert(target.end(), std::make_move_iterator(input.begin()), std::make_move_iterator(input.end()));
}

std::optional<FSNodes> Folder::get_contents_recursively(const std::atomic<bool>& stop_token /*= false*/) const
{
	FSNodes nodes;
	FSNodes local_folders;
	FSNodes local_files;

	WIN32_FIND_DATA findFileData;
															
	std::queue<std::wstring> tasks_pool;
		
	tasks_pool.push(m_path_);

	while (!tasks_pool.empty() and !stop_token)
	{
		HANDLE hFind = FindFirstFile(std::wstring(tasks_pool.front().data() + std::wstring(L"\\*")).c_str(), &findFileData);

		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (std::wstring fileName = findFileData.cFileName; fileName != L"." && fileName != L".." && fileName != L"")
				{
					std::wstring formattedPath = formatPath(fileName, tasks_pool.front());

					if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						tasks_pool.emplace(formattedPath);
						local_folders.emplace_back(std::make_unique<Folder>(std::move(formattedPath)));
						continue;
					}
					local_files.emplace_back(std::make_unique<File>(std::move(formattedPath)));
				}
			} while (FindNextFile(hFind, &findFileData));
		}
		FindClose(hFind);
		tasks_pool.pop();
	}

	bool high_elem_count = (local_folders. size() + local_files.size()) > 100'000;

	std::variant<std::execution::sequenced_policy, std::execution::parallel_policy> exec_policy;

	if (high_elem_count)
		exec_policy = std::execution::par;
	else
		exec_policy = std::execution::seq;

	visit_vector(exec_policy, local_folders);

	visit_vector(exec_policy, local_files);

	nodes.reserve(local_files.size() + local_folders.size());

	move_into_vector(nodes, local_folders);
	move_into_vector(nodes, local_files);

	if (nodes.empty())
		return std::nullopt;
	return nodes;
}



std::optional<FSNodes> Folder::get_contents(const std::atomic<bool>& stop_token /*= false*/) const
{
	FSNodes nodes;

	FSNodes local_folders;

	FSNodes local_files;

	WIN32_FIND_DATA findFileData;

	HANDLE hFind = FindFirstFile(std::wstring(m_path_ + std::wstring(L"\\*")).c_str(), &findFileData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (std::wstring fileName = findFileData.cFileName; fileName != L"." && fileName != L".." && fileName != L"")
			{
				std::wstring formattedPath = formatPath(fileName, m_path_);

				if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					local_folders.emplace_back(std::make_unique<Folder>(std::move(formattedPath)));
				else
					local_files.emplace_back(std::make_unique<File>(std::move(formattedPath)));
			}
		} while (FindNextFile(hFind, &findFileData) && !stop_token);
	}
	FindClose(hFind);

	bool high_elem_count = (local_folders.size() + local_files.size()) > 100'000;

	std::variant<std::execution::sequenced_policy, std::execution::parallel_policy> exec_policy;

	if (high_elem_count)
		exec_policy = std::execution::par;
	else
		exec_policy = std::execution::seq;

	visit_vector(exec_policy, local_folders);
	visit_vector(exec_policy, local_files);

	nodes.reserve(local_files.size() + local_folders.size());
	
	move_into_vector(nodes, local_folders);
	move_into_vector(nodes, local_files);

	if (nodes.empty())
		return std::nullopt;

	return nodes;
}