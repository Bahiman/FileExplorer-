#include"../headers/Algorithms.hpp"
#include"../headers/Folder.hpp"
#include"../headers/File.hpp"
#include<execution>
#include<fstream>
#include <vector>
#include "../utilities/Timer.hpp"


using u_FileSystemNode = std::unique_ptr<FileSystemNode>;

using s_FileSystemNode = std::shared_ptr<FileSystemNode>;

using FileSystemNodeVec = std::vector<u_FileSystemNode>;

__forceinline bool matches(const FileSystemNode& node, std::wstring_view keyword)
{
	auto file_name = node.get_file_name();

	return file_name.find(keyword) != std::wstring::npos;
}

__forceinline std::unique_ptr<FileSystemNode> get_copy(const FileSystemNode* elem)
{
	if (auto folder_conversion = dynamic_cast<const Folder*>(elem))
		return std::unique_ptr<Folder>(new Folder(*folder_conversion,1));
	else
	{
		auto file_conversion = dynamic_cast<const File*>(elem);
		return std::unique_ptr<File>(new File(*file_conversion,1));
	}
}

std::optional<FSNodes> algorithms::find_by_keyword(FSNodes & nodes,
std::wstring_view keyword, const std::atomic<bool>& stop_token)
{
	TIMER

	FSNodes nodes_to_return;

	std::vector<size_t> indeces;

	for (size_t i = 0; i < nodes.size(); i++, !stop_token)
	{
		if (matches(*(nodes[i]), keyword))
			indeces.push_back(i);
	}

	std::ranges::for_each(indeces, [&](size_t integer) {nodes_to_return.emplace_back(get_copy((nodes[integer].get()))); });
	
	if (nodes_to_return.empty())
		return std::nullopt;
	return nodes_to_return;
}

