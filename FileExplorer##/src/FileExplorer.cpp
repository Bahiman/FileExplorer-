#include "../headers/FileExplorer.hpp"
#include "../headers/Folder.hpp"
#include "../headers//Algorithms.hpp"
#include <QFileInfo>
#include <iostream>
#include<fstream>

FileExplorer::FileExplorer(QWidget *parent) : QMainWindow(parent)
{
	std::mutex cout_mutex;
	std::thread thread_one
	(
		[&]
		{

			Folder folder("C:\\");

			auto res = folder.get_contents();

			if (res)
			{
				auto result = algorithms::find_by_keyword(res.value(), L"Program");

				std::fstream stream("log.txt");


				{
					std::scoped_lock lock(cout_mutex);
					std::cout << "||||||||||||||||||||||||||||||||||||||" << std::endl;
				}


				if (result.has_value())
				{
					std::ranges::for_each(result.value(), [&](FSNodes::const_iterator iterator) {
						std::scoped_lock lock(cout_mutex);
						std::wcout << (*(*iterator)).get_path() << '\n'; 
					});
				}
				else
				{
					std::cout << "Nothing found" << std::endl;
				}
			}
		}
	);

	std::thread thread_two
	(
		[&]
		{

			Folder folder("D:\\");

			auto res = folder.get_contents();

			if (res)
			{
				auto result = algorithms::find_by_keyword(res.value(), L"Program");

				std::fstream stream("log.txt");

				{
					std::scoped_lock lock(cout_mutex);
					std::cout << "||||||||||||||||||||||||||||||||||||||" << std::endl;
				}

				if (result.has_value())
				{
					std::ranges::for_each(result.value(), [&](FSNodes::const_iterator iterator) {
						std::scoped_lock lock(cout_mutex);
						std::wcout << (*(*iterator)).get_path() << '\n'; }
					);
				}
				else
				{
					std::cout << "Nothing found" << std::endl;
				}
			}
		}
	);

	thread_one.join();
	thread_two.join();
}

FileExplorer::~FileExplorer() = default;
