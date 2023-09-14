#include "../headers/Folder.hpp" 
#include "../headers/File.hpp"
#include "../utilities/Timer.hpp"
#include "../utilities/CancellableThreadPool.hpp"
#include <Windows.h>


uint64_t Folder::get_size(const std::atomic<bool>& p_continue) const
{
	auto contents = get_contents_recursively();

	if (!contents)
		return 0;

	uint64_t result = 0;

	std::ranges::for_each(contents.value(), [&result](auto& ptr) {result += ptr->get_size(); });

	return result;
}

std::optional<FSNodes> Folder::get_contents_recursively(const std::atomic<bool>& stop_token) const
{
	FSNodes nodes;

	FSNodes local_folders;

	FSNodes local_files;

	auto formatPath = [](std::wstring_view file_name, std::wstring directory) -> std::wstring
	{
		if (directory.back() != '\\') // \ 
			directory.append(1, '\\');
		directory.append(file_name);

		return directory;
	};

	WIN32_FIND_DATA findFileData;

	std::queue<std::wstring> tasks_pool;

	tasks_pool.push(m_path_);

	while (!tasks_pool.empty() && !stop_token)
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
						tasks_pool.push(formattedPath);

						std::erase(formattedPath, '*');

						local_folders.emplace_back(std::make_unique<Folder>(std::move(formattedPath)));
					}
					else
					{
						std::erase(formattedPath, '*');

						local_files.emplace_back(std::make_unique<File>(std::move(formattedPath)));
					}
				}
			} while (FindNextFile(hFind, &findFileData));
		}
		FindClose(hFind);
		tasks_pool.pop();
	}

	auto fix_file_vector = std::async(std::launch::async, [&local_files] {
		for (auto& elem : local_files)
		{
			std::wstring acquired_path = elem->get_path().wstring();

			std::erase(acquired_path, L'*');

			elem->set_path(std::move(acquired_path));
		}
		}
	);

	auto fix_folder_vector = std::async(std::launch::async, [&local_folders] {
		for (auto& elem : local_folders)
		{
			std::wstring acquired_path = elem->get_path().wstring();

			std::erase(acquired_path, L'*');

			elem->set_path(std::move(acquired_path));
		}
		}
	);

	nodes.reserve(local_files.size() + local_folders.size());

	fix_folder_vector.get();
	nodes.insert(nodes.end(), std::make_move_iterator(local_folders.begin()), std::make_move_iterator(local_folders.end()));

	fix_file_vector.get();
	nodes.insert(nodes.end(), std::make_move_iterator(local_files.begin()), std::make_move_iterator(local_files.end()));

	if (nodes.empty())
		return std::nullopt;	

	return nodes;
}
std::optional<FSNodes> Folder::get_contents(const std::atomic<bool>& stop_token) const
{
	FSNodes nodes;

	FSNodes local_folders;

	FSNodes local_files;

	auto formatPath = [](std::wstring_view file_name, std::wstring directory) -> std::wstring
		{
			if (directory.back() != '\\') // \ 
				directory.append(1, '\\');
			directory.append(file_name);

			return directory;
		};

	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile(std::wstring(m_path_.wstring() + std::wstring(L"\\*")).c_str(), &findFileData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (std::wstring fileName = findFileData.cFileName; fileName != L"." && fileName != L".." && fileName != L"")
			{
				std::wstring formattedPath = formatPath(fileName, m_path_);

				if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{

					std::erase(formattedPath, '*');

					local_folders.emplace_back(std::make_unique<Folder>(std::move(formattedPath)));
				}
				else
				{
					std::erase(formattedPath, '*');

					local_files.emplace_back(std::make_unique<File>(std::move(formattedPath)));
				}
			}
		} while (FindNextFile(hFind, &findFileData));
	}
	FindClose(hFind);

	auto fix_file_vector = std::async(std::launch::async, [&local_files] {
		for (auto& elem : local_files)
		{
			std::wstring acquired_path = elem->get_path().wstring();

			std::erase(acquired_path, L'*');

			elem->set_path(std::move(acquired_path));
		}
		}
	);

	auto fix_folder_vector = std::async(std::launch::async, [&local_folders] {
		for (auto& elem : local_folders)
		{
			std::wstring acquired_path = elem->get_path().wstring();

			std::erase(acquired_path, L'*');

			elem->set_path(std::move(acquired_path));
		}
		}
	);
	nodes.reserve(local_files.size() + local_folders.size());

	fix_file_vector.get();

	fix_folder_vector.get();

	nodes.insert(nodes.end(), std::make_move_iterator(local_folders.begin()), std::make_move_iterator(local_folders.end()));

	nodes.insert(nodes.end(), std::make_move_iterator(local_files.begin()), std::make_move_iterator(local_files.end()));

	if (nodes.empty())
		return std::nullopt;

	return nodes;
}


   