#pragma once
#include <QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListView>
#include <QLineEdit>
#include <QStringListModel>
#include <QIcon>
#include "ui_FileExplorer.h"
#include "../headers/FileListModel.hpp"

class FileExplorer : public QMainWindow
{
    Q_OBJECT

public:
    FileExplorer(QWidget *parent = nullptr);
    ~FileExplorer() override;
private:
    Ui::FileExplorerClass ui;
    QVBoxLayout* mainLayout;
    QHBoxLayout* topLayout;
    QPushButton* back_button;
    QPushButton* forward_button;
    QLineEdit* file_browser_line;
    QListView* elem_view;
    FSNodes* current_nodes;
    FileListModel* model;
    std::filesystem::path current_path;
    void setUi();
    void setConnects();
    void setButtons();
    void setFileBrowser();
    void setSearchLine();
    void setListView();
};
