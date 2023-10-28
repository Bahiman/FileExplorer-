#include"../headers/Algorithms.hpp"
#include"../headers/Folder.hpp"
#include"../headers/File.hpp"
#include<execution>
#include<fstream>
#include <iostream>
#include <vector>
#include <execution>
#include <unordered_set>
#include<map>

std::optional<FSNodes> algorithms::find_by_keyword(FSNodes & nodes, std::wstring_view keyword, const std::atomic<bool>& stop_token)
//std::optional<FSNodes> algorithms::find_by_keyword(FSNodes & nodes, std::wstring_view keyword, const std::atomic<bool>& stop_token)
{
	uint64_t integer = 0;

	FSNodes result;

	FSNodes::iterator it = nodes.begin();
	auto matches = [&](const FileSystemNode& node, std::wstring_view keyword) {
		auto file_name = node.get_file_name();

		size_t pos = file_name.find(keyword);

		bool result = pos != std::wstring::npos;

		//std::cout << result << '\n';

		return result;
	};

	std::cout << nodes.size() << " !!@#" << std::endl;

	auto res = std::find_if(std::execution::par_unseq, it ,nodes.end(),
		[&](const std::unique_ptr<FileSystemNode>& file_node) {
			return matches(*file_node, keyword);
	});

	it++;

	auto get_copy = [&](FSNodes::const_iterator it) -> std::unique_ptr<FileSystemNode>{
		if (auto res = dynamic_cast<File*>((*it).get()))
			return std::unique_ptr<FileSystemNode>(new File(*res, 1)); // using the second integer as the compiler confused the class for the templated path

		if (auto res = dynamic_cast<Folder*>((*it).get()))
			return std::unique_ptr<FileSystemNode>(new Folder(*res, 1));

		return nullptr;
	};
	
	std::wcout << keyword << " is the keyword\n";

	std::ofstream log("could.txt", std::ios::trunc); // true means that the file was a path 

	//std::map<std::wstring_view, bool> map;

	while (res != nodes.end() && it != nodes.end() && !stop_token)
	{
		result.emplace_back(get_copy(it));
		it++;
		res = std::find_if(std::execution::par_unseq, it, nodes.end(), [&](const std::unique_ptr<FileSystemNode>& file_node) {
			return matches(*file_node, keyword); 
		}); // what does the fox say 
	}
	if (result.empty())
		return std::nullopt;
	return result;
}
