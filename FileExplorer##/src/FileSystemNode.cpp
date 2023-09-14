#include "../headers/FileSystemNode.hpp"

std::string FileSystemNode::get_creation_date(const Path& p_path) const
{
	QFileInfo m_file_info(m_path_);

	return m_file_info.birthTime().toString().toStdString();
}

std::string FileSystemNode::get_last_access_date(const Path& p_path) const
{
	QFileInfo m_file_info(m_path_);

	return m_file_info.lastRead().toString().toStdString();
}

std::string FileSystemNode::get_last_modification_date(const Path& p_path) const
{
	QFileInfo m_file_info(m_path_);

	return m_file_info.lastModified().toString().toStdString();
}

const Path& FileSystemNode::get_path() const
{
	return m_path_;
}

