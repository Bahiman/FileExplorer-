#include "../headers/File.hpp"

#include <cstdint>

uint64_t File::get_size(const std::atomic<bool>& p_continue) const
{
	return std::filesystem::file_size(m_path_);		
}
