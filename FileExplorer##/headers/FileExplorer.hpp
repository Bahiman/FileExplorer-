#pragma once

#include "ui_FileExplorer.h"

class FileExplorer : public QMainWindow
{
    Q_OBJECT

public:
    FileExplorer(QWidget *parent = nullptr);
    ~FileExplorer() override;

private:
    Ui::FileExplorerClass ui;
};
