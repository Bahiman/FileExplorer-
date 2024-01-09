#pragma once
#include <QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include<qtimer.h>
#include <QVBoxLayout>
#include <QListView>
#include <QLineEdit>
#include <QStringListModel>
#include <QIcon>
#include "ui_FileExplorer.h"
#include "../headers/FileListModel.hpp"

// C:\Program Files\PowerToys\modules\FileExplorerPreview

// the level of the message provided to the dialogue box
enum class DLG_BX
{
    INFO,
    WARNING,
    ERROR
};

// A class to `call` the atomic bool when the scope is over
// (used to modify the bool after the function rerturned)
class SearchManage
{
private:
    std::atomic<bool>& m_atomic_bool;
public:
    SearchManage(std::atomic<bool>& atomic_bool) : m_atomic_bool(atomic_bool) {};
    ~SearchManage()
    {
        m_atomic_bool = true;
    }
};


class FileExplorer : public QMainWindow
{
    Q_OBJECT
public:
    FileExplorer(QWidget *parent = nullptr);
    std::optional<FSNodes> search_function();
    void do_every_time();
    //~FileExplorer() override;
private:
    Ui::FileExplorerClass ui;
    FileListModel* model; // My custom model for the list-view
    std::optional<FSNodes> m_contents; // Current contents
    std::wstring current_path; // current path(redundant) 
    bool search_thread_init; // a flag that is used to check whether the search has been init.
    bool file_browser_text_changed; 
    bool search_line_text_changed;
    std::atomic<bool> search_over = false;
    std::atomic<bool> search_stop_token = false;
    std::atomic<bool> search_valid = false; // Checks if the search is valid(it may invalidated if the user has hit cancel)
    std::future<std::optional<FSNodes>> search_thread; // the thread where the search is done 
    QTimer* job_done_timer = new QTimer(this); // A timer that will call the `do_every_time_function` every .5 seconds
    bool eventFilter(QObject* watched, QEvent* event) override;
    void setConnects();
    void setUi();
    void setEventFilter(); // setting the event filter to keep track of the alterations in the ui
    void startSearch(); // function that is called when the user has initiated the search
    void elemClicked();
    void backButtonClicked();
    void updateContents(const Folder& folder); // update the contents to a given folder
    void updateUiToConents(bool keep_path = false); // update the ui to contents
    void openDialogueBox(DLG_BX level, std::wstring_view message); // opening a dialogue box
    
};
