#include "../headers/FileSystemNode.hpp"

std::string FileSystemNode::get_creation_date(const std::filesystem::path& p_path) const
{
	return m_file_info.birthTime().toString().toStdString();
}

std::string FileSystemNode::get_last_access_date(const std::filesystem::path& p_path) const
{
	return m_file_info.lastRead().toString().toStdString();
}

std::string FileSystemNode::get_last_modification_date(const std::filesystem::path& p_path) const
{
	return m_file_info.lastModified().toString().toStdString();
}

