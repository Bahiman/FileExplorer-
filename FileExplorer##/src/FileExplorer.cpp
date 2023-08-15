#include "../headers/FileExplorer.hpp"
#include "../headers/Folder.hpp"
#include <QFileInfo>
#include <iostream>

FileExplorer::FileExplorer(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    QFileInfo info("C:\\Windows");
    Folder folder(L"C:\\Windows");
    std::cout << folder.get_size(true) << std::endl;
}

FileExplorer::~FileExplorer() = default;
