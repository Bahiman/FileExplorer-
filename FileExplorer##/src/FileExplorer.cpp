#include "../headers/FileExplorer.hpp"
#include "../headers/Folder.hpp"
#include "../headers//Algorithms.hpp"
#include<Windows.h>
#include <QFileInfo>
#include <iostream>
#include<fstream>
#include <Windows.h>
#include <iostream>
#include<syncstream>
#include <string>
#include "../utilities/Timer.hpp"
#include "../headers/File.hpp"
#include <execution>
#include <codecvt> 
#include <numbers>

enum myEnum
{
	Bahram
};

template<typename T>
class Vector
{
public:

    const size_t Size() const
    {
        return size;
    }

    T& operator[](const size_t index)
    {
        return arr[index];
    }

    void Reserve(const size_t reserve_size)
    {

        if (arr != nullptr)
        {
            arr = (T*)realloc(arr, reserve_size * sizeof(T));

            capacity = reserve_size;
        }
        else
        {
            arr = new T[reserve_size];

            capacity = reserve_size;
        }
    }

    void Push(T var)
    {
        if (size + 1 > capacity)
        {
            Reserve(static_cast<size_t>((size + 1) * 2));
            arr[size] = var;
            ++size;
        }
        else
        {
            arr[size] = var;
            ++size;
        }
    }

    Vector()
    {
        size = 0;

        capacity = 0;
        arr = nullptr;
    }

    void forEach(std::function<void(T&)> fun)
    {
        if (fun != nullptr)
        {
            for (int i = 0; i < size; i++)
            {
                fun(arr[i]);
            }
        }
        else
        {
            std::cout << "The function is null!\n";
        }
    }

    class Iterator {
    private:
        T* ptr;
    public:
        Iterator(T* pointer) : ptr(pointer) {}

        // Define the necessary iterator operations
        T& operator*() const { return *ptr; }
        T* operator->() const { return ptr; }
        Iterator& operator++() {
            ptr++;
            return *this;
        }
        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }
        bool operator==(const Iterator& other) const {
            return ptr == other.ptr;
        }
        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };

    bool find(T& arg)
    {
        for (int i = 0; i < size; i++)
        {
            if (arg == arr[i]) return true;
        }

        return false;
    }

    bool find(T arg)
    {
        for (int i = 0; i < size; i++)
        {
            if (arg == arr[i]) return true;
        }

        return false;
    }

    bool find(T& arg, std::function<bool(T&)> funct)
    {
        for (int i = 0; i < size; i++)
        {
            if (funct(arr[i])) return true;
        }

        return false;
    }

    bool find(T arg, std::function<bool(T&)> funct)
    {
        for (int i = 0; i < size; i++)
        {
            if (funct(arr[i])) return true;
        }

        return false;
    }

    // Functions to obtain begin and end iterators
    Iterator begin() { return Iterator(arr); }
    Iterator end() { return Iterator(arr + size); }

    using value_type = T;

private:
    T* arr;
    size_t capacity;
    size_t size;
};

std::vector<std::wstring> strings = {};

int32_t vectors_index = 0;

template <class T>
class VecSupport
{
private:
   std::vector<T> m_vec;
public:
    VecSupport<T>(size_t size)
    {
        m_vec = std::vector<T>(size);
    }
    VecSupport<T>()
    {
        m_vec = std::vector<T>();
    }   
    std::vector<T>& get_vec()
    {
        return m_vec;
    }
    
    T& at(intmax_t index)
    {
        if (index < 0)
        {
            intmax_t estimated_index = m_vec.size() - (-1 * index);

            return m_vec[estimated_index];
        }
        else
        {
            m_vec[index];
        }
    }
};

void ListFilesRecursively(const std::wstring& directory)
{
    auto formatPath = [](std::wstring_view file_name, std::wstring directory) -> std::wstring
    {
       if (directory.back() != '\\') // \ 
           directory.append(1, '\\');

       directory.append(file_name);

       return directory;
    };

    WIN32_FIND_DATA findFileData;

    std::queue<std::wstring> tasks_pool;

    tasks_pool.push(directory);

    while (!tasks_pool.empty())
    {
        HANDLE hFind = FindFirstFile(std::wstring(tasks_pool.front().data() + std::wstring(L"\\*")).c_str(), &findFileData);

        if (hFind != INVALID_HANDLE_VALUE)
        {
            do
            {
                if (std::wstring fileName = findFileData.cFileName; fileName != L"." && fileName != L".." && fileName != L"")
                {
                    std::wstring formattedPath = formatPath(fileName, tasks_pool.front());

                    if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    {
                        strings.push_back(formattedPath);

                        tasks_pool.emplace(std::move(formattedPath));
                    }
                    else 
                    {
                        strings.emplace_back(std::move(formattedPath));
                    }
                }
            } while (FindNextFile(hFind, &findFileData));
        }
        FindClose(hFind);
        tasks_pool.pop();
    }
}

FileExplorer::FileExplorer(QWidget* parent) : QMainWindow(parent)
{
    try
    {
		Timer t;
		ui.setupUi(this);

		model = new FileListModel(this);

		Folder folder("C:\\");

		auto value = folder.get_contents_recursively();

		t.~Timer();

		if (value)
		{
			for (auto& a : value.value())
			{
				if (std::filesystem::is_directory(a->get_path()))
				{
					model->addData(Folder(a->get_moveable_path()));
				}
				else
				{
					model->addData(File(a->get_moveable_path()));
				}
			}
		}
		ui.file_view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		ui.file_view->setModel(model);
		ui.file_view->updateGeometry();
		ui.file_view->show();
    }
    catch (std::exception& e)
    {
        std::wcout << e.what() << std::endl;
    }

}

FileExplorer::~FileExplorer() = default;
	