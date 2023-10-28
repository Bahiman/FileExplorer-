#pragma once
#include <QApplication>
#include <QListView>
#include <QListView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QDir>
#include <QFileInfo>
#include "File.hpp"
#include "Folder.hpp"
#include <QIcon>

class FileListModel : public QAbstractListModel
{
public:
	explicit FileListModel(const FileSystemNode& file, QObject* parent = nullptr);
	explicit FileListModel(QObject* parent = nullptr);
	int32_t rowCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	void addData(const FileSystemNode& file, bool keep_path);
	void clear();
	Qt::ItemFlags flags(const QModelIndex& index) const;
private:
	struct FileItem
	{
		QIcon icon;
		QString name;
	};
	QList <FileItem> files_;
};