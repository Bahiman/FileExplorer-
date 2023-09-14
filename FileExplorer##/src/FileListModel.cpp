#include "../headers/FileListModel.hpp"
#include <QApplication>
#include <QStyle>
#include <QStyleFactory>
#include <iostream>
FileListModel::FileListModel(const FileSystemNode& file, QObject* parent)
{
	setParent(parent);
	FileItem file_item;
	file_item.name = QString::fromStdString(file.get_path().string().substr(file.get_path().string().find_last_of('/') + 1));
	// this will be finished later
	file_item.icon = QApplication::style()->standardIcon(QStyle::SP_FileIcon);
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

//void FileListModel::addFile(const FileSystemNode* file)
//{
//	FileItem file_item;
//	file_item.name = QString::fromStdString(file.getPath().string().substr(file.getPath().string().find_last_of('/') + 1));
//	file_item.icon = QApplication::style().standardIcon(QStyle::SP_FileIcon);
//	files_.push_back(file_item);
//}

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

void FileListModel::addData(const FileSystemNode& file)
{
	//FileItem file_item;
	//file_item.name = QString::fromStdString(file.getPath().string().substr(file.getPath().string().find_last_of('/') + 1));
	//file_item.icon = QApplication::style()->standardIcon(QStyle::SP_FileIcon);
	//files_.push_back(file_item);
	//std::cout << file_item.name.toStdString() << std::endl;
	beginInsertRows(QModelIndex(), files_.size(), files_.size());
	FileItem file_item;
	file_item.name = QString::fromStdWString(file.get_path().wstring().substr(file.get_path().string().find_last_of('/') + 1));
	file_item.icon = QApplication::style()->standardIcon(QStyle::SP_FileDialogNewFolder);
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

