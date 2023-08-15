#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_FileExplorer.h"

class FileExplorer : public QMainWindow
{
    Q_OBJECT

public:
    FileExplorer(QWidget *parent = nullptr);
    ~FileExplorer();

private:
    Ui::FileExplorerClass ui;
};
