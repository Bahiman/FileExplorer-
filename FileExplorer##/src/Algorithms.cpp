#include"../headers/Algorithms.hpp"

inline bool matches(const FileSystemNode& node, std::wstring_view keyword)
{
	const auto& file_name = node.get_path().filename();

	size_t pos = file_name.wstring().find(keyword);

	return pos != std::wstring::npos;
}

std::optional<std::vector<FSNodes::const_iterator>> algorithms::find_by_keyword(const FSNodes& nodes, std::wstring_view keyword, const std::atomic<bool>& stop_token)
{
	auto it = nodes.begin();
	const auto end = nodes.end();
	std::vector<FSNodes::const_iterator> result;
	while (it != end && !stop_token)
	{
		if (matches(**it, keyword))
			result.push_back(it);
		++it;
	}
	if (result.empty())
		return std::nullopt;
	return result;
}
