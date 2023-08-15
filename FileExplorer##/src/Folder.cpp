#include"../headers/Folder.hpp"
#include"../headers/File.hpp"
#include "../utilities/CancellableThreadPool.hpp"

int64_t Folder::get_size(const std::atomic<bool>& p_continue) const
{
	return 0;
}

std::vector<std::unique_ptr<FileSystemNode>> Folder::get_all_contents(const std::atomic<bool>& stop_token)
{
	auto dir_iterator = std::filesystem::directory_iterator(m_path_);

	std::mutex nodes_mutex;

	std::mutex pool_mutex;	 

	std::vector<std::future<void>> tasks;

	std::vector<std::unique_ptr<FileSystemNode>> nodes;

	std::vector<std::unique_ptr<FileSystemNode>> local_nodes;

	ThreadPool pool(std::thread::hardware_concurrency());

	while(dir_iterator != std::filesystem::directory_iterator() && !stop_token)
	{
		if(const auto& entry = *dir_iterator; entry.is_regular_file())
		{
			local_nodes.emplace_back(std::make_unique<File>(entry.path()));
		}
		else
		{
			
		}
		++dir_iterator;
	}
}
