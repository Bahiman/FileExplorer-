#include "../headers/FileListModel.hpp"
#include<windows.h>
#include<fileapi.h>
#include <QApplication>
#include <QStyle>
#include <QStyleFactory>
#include <iostream>


FileListModel::FileListModel(const FileSystemNode& file, QObject* parent)
{
	setParent(parent);
	FileItem file_item;
	file_item.name = QString::fromStdWString(file.get_path().substr(file.get_path().find_last_of('/') + 1));
	// this will be finished later

	file_item.icon = QApplication::style()->standardIcon(QStyle::SP_ArrowRight);
	files_.push_back(file_item);
}

FileListModel::FileListModel(QObject* parent)
{
	setParent(parent);
}

int FileListModel::rowCount(const QModelIndex& parent) const
{
	return files_.size();
}


QVariant FileListModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role == Qt::DisplayRole)
		return files_.at(index.row()).name;
	else if (role == Qt::DecorationRole)
		return files_.at(index.row()).icon;

	return QVariant();
}

void FileListModel::addData(const FileSystemNode& file, bool keep_path = false)
{
	beginInsertRows(QModelIndex(), files_.size(), files_.size());
	FileItem file_item;
	auto file_is_folder = [&file]() -> bool
	{
		return dynamic_cast<const Folder*>(&file);
	};
	file_item.name =  !keep_path ? QString::fromStdWString(file.get_file_name().data()) : QString::fromStdWString(file.get_path());
	//std::wcout << file.get_file_name().size() << " ???" << std::endl;
	file_item.icon = file_is_folder() ? QApplication::style()->standardIcon(QStyle::SP_DirIcon) : QApplication::style()->standardIcon(QStyle::SP_FileIcon);
	files_.push_back(file_item);
	endInsertRows();
}

Qt::ItemFlags FileListModel::flags(const QModelIndex& index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

void FileListModel::clear()
{
	files_.clear();
}

