#include "../headers/FileSystemNode.hpp"
#include <iostream>

std::string FileSystemNode::get_creation_date(const Path& p_path) const
{
	QFileInfo m_file_info(QString::fromStdWString(m_path_));

	return m_file_info.birthTime().toString().toStdString();
}



std::string FileSystemNode::get_last_access_date(const Path& p_path) const
{
	QFileInfo m_file_info(QString::fromStdWString(m_path_));

	return m_file_info.lastRead().toString().toStdString();
}

std::string FileSystemNode::get_last_modification_date(const Path& p_path) const
{
	QFileInfo m_file_info(QString::fromStdWString(m_path_));

	return m_file_info.lastModified().toString().toStdString();
}

const Path& FileSystemNode::get_path() const
{
	return m_path_;
}

std::wstring_view FileSystemNode::get_file_name() const
{
	auto filePos = m_path_.rfind(L'\\');

	if (filePos != std::wstring::npos)
		++filePos;

	std::wstring_view path_view = m_path_;

	std::wstring_view view = path_view.substr(filePos);

	return view;
}

