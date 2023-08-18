#pragma once
#include<optional>
#include<vector>
#include "../headers/FileSystemNode.hpp"

namespace algorithms
{
	[[nodiscard]] std::optional<std::vector<FSNodes::const_iterator>> find_by_keyword(const FSNodes& nodes, std::wstring_view keyword, const std::atomic<bool>& stop_token = false);
}