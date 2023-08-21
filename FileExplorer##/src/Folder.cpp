#include "../headers/Folder.hpp" 
#include "../utilities/Timer.hpp"
#include "../utilities/CancellableThreadPool.hpp"
#include <iostream>
#include<Windows.h>

inline FSNodes m_get_nodes_recursively(ThreadPool& pool, std::recursive_mutex& pool_mutex, Path path, std::vector<std::future<void>>& tasks , const std::atomic<bool>& stop = false)
{
    
}

uint64_t Folder::get_size(const std::atomic<bool>& p_continue) const
{
	return 0;
}

std::optional<FSNodes> Folder::get_contents(const std::atomic<bool>& stop_token) const
{
	
	//TIMER

	//ThreadPool pool(std::thread::hardware_concurrency() / 2);

	//std::vector<std::future<FSNodes>> tasks;

	//FSNodes local_nodes;

	//auto dir_iterator = std::filesystem::directory_iterator(m_path_);

	//const auto end = std::filesystem::directory_iterator();

	//while (dir_iterator != end && !stop_token)
	//{
 //    if(const auto& entry = *dir_iterator; entry.is_regular_file() && !entry.is_directory())
	//	local_nodes.emplace_back(std::make_unique<File>(entry.path()));
	// else if (!entry.is_regular_file() && entry.is_directory())
	// {
	//	tasks.emplace_back(pool.enqueue(get_nodes, entry.path(), std::ref(stop_token)));
	// }
	// ++dir_iterator;
	//}

	//for(auto& task : tasks)
	//{
	//	FSNodes res = task.get();

	//	local_nodes.insert(local_nodes.end(), std::make_move_iterator(res.begin()), std::make_move_iterator(res.end()));
	//}

	//if (local_nodes.empty())
	//	return std::nullopt;
	//return local_nodes;

	TIMER

	ThreadPool pool(std::thread::hardware_concurrency());

	FSNodes nodes;


}
