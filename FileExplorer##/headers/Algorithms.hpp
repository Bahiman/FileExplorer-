#pragma once
#include<optional>
#include<vector>
#include "../headers/FileSystemNode.hpp"

namespace algorithms
{
	[[nodiscard]] std::optional<FSNodes> find_by_keyword(FSNodes & nodes, std::wstring_view keyword, const std::atomic<bool>& stop_token = false);
}