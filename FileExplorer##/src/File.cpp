#include "../headers/File.hpp"

#include <cstdint>

int64_t File::get_size(const std::atomic<bool>& p_continue) const
{
	return m_file_info.size();
}

File::File(const File& file) noexcept : FileSystemNode(file.m_path_) 
{
	this->m_file_info = file.m_file_info;
}

File::File(File&& file) noexcept
{
	this->m_path_ = std::move(file.m_path_);
	this->m_file_info = std::move(file.m_file_info);
}

File& File::operator=(const File& file) noexcept
{
	this->m_path_ = file.m_path_;
	this->m_file_info = file.m_file_info;
	return *this;
}
