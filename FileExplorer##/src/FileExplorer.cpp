#include"../headers/FileExplorer.hpp"
#include"../headers/Algorithms.hpp"
#include<qevent.h>
#include<iostream>
#include "../utilities/Timer.hpp"


// a function that checks 
void FileExplorer::do_every_time()
{
	if (search_over && search_valid)
	{
		auto search_res = search_thread.get();
		if (search_res)
		{
			m_contents = std::move(search_res);
		}
		else
		{
			m_contents = std::nullopt;
		}
		updateUiToConents(true);
		search_over = false;
	}
}

FileExplorer::FileExplorer(QWidget* parent)
{
	setUi();
	setConnects();
	setEventFilter();
	current_path = L"C:\\";
	ui.file_browser->setText(QString::fromStdWString(current_path));
}

bool FileExplorer::eventFilter(QObject* watched, QEvent* event)
{
	if (watched == ui.file_browser && event->type() == QEvent::KeyPress) {
		QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
		if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) {
			if (file_browser_text_changed) {
				Folder folder(ui.file_browser->text().toStdWString());
				updateContents(folder);
				updateUiToConents();
				search_stop_token = false;
				search_valid = false;
				file_browser_text_changed = false;
			}
		}
	}
	else if (watched == ui.search_line && event->type() == QEvent::KeyPress)
	{
		QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

		if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) {
			if (search_line_text_changed) {
				updateUiToConents();
				search_valid = false;
				startSearch();
				search_line_text_changed = false;
			}
		} 
	}


	return false;
}

void FileExplorer::setConnects()
{
	connect(job_done_timer, &QTimer::timeout, this, &FileExplorer::do_every_time);
	connect(ui.file_browser, &QLineEdit::textChanged, this, [&]() {
		this->file_browser_text_changed = true;
		});
	connect(ui.file_view, &QListView::clicked, this, &FileExplorer::elemClicked);
	connect(ui.back_button, &QPushButton::clicked, this, &FileExplorer::backButtonClicked);
	connect(ui.search_line, &QLineEdit::textChanged, this, [this]() {
		this->search_line_text_changed = true;
		});
}

void FileExplorer::setUi()
{
	ui.setupUi(this);
	ui.file_view->setModel(model = new FileListModel(this));
}

void FileExplorer::setEventFilter()
{
	ui.file_browser->installEventFilter(this);

	ui.search_line->installEventFilter(this);

	job_done_timer->start(500);
}

std::optional<FSNodes> FileExplorer::search_function()
{
	SearchManage s(search_over);
	Folder work_folder(ui.file_browser->text().toStdWString());
	Timer t;
	auto res = work_folder.get_contents_recursively(search_stop_token);
	t.~Timer();
	std::cout << "Got the goodies\n";
	if (res)
	{
		Timer tt;
		auto search_res = algorithms::find_by_keyword(*res, ui.search_line->text().toStdWString(), search_stop_token);
		tt.~Timer();
		std::cout << "Got the milkies!\n";
		if (!search_res)
		{
			openDialogueBox(DLG_BX::INFO, L"Nothing found");
			return std::nullopt;
		}
		return search_res;
	}
	return std::nullopt;
}

void FileExplorer::startSearch()
{
	std::cout << "Search Started" << std::endl;
	search_valid = true;
	if (search_thread_init)
	{
		search_stop_token = true;
		if(search_thread.valid())
			(void) search_thread.get();
		search_stop_token = false;
	}
	search_thread = std::async(std::launch::async, [this]() -> std::optional<FSNodes> {
		return search_function();
	});
	search_thread_init = true;
}

void FileExplorer::elemClicked()
{

}

void FileExplorer::backButtonClicked()
{

}

void FileExplorer::updateContents(const Folder& folder)
{
	m_contents = folder.get_contents();
}

void FileExplorer::updateUiToConents(bool keep_path)
{
	if (!m_contents)
		openDialogueBox(DLG_BX::INFO, L"Nothing Found");

	model->clear();

	if (m_contents)
	{
		std::ranges::for_each(m_contents.value(), [&](auto& node) {
			model->addData(*node,keep_path);
		});
		return;
	}
	std::cout << "Nothing to deduce" << std::endl;
}
void FileExplorer::openDialogueBox(DLG_BX level, std::wstring_view message)
{
}

