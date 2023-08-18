#pragma once
#include "FileSystemNode.hpp"
#include"../utilities/CancellableThreadPool.hpp"


// A type alias for a vector of futures for the ease of use
using TasksVec = std::vector<std::future<void>>;

class Folder : public FileSystemNode
{
public:
	//template<typename T, typename = std::enable_if<std::is_convertible_v<T, Path>>>
	//explicit Folder(T&& path) : FileSystemNode(std::forward<T>(path)) {}
	~Folder() override = default;
	[[nodiscard]] uint64_t get_size(const std::atomic<bool>& p_continue = false) const override;
	[[nodiscard]] std::optional<FSNodes> get_contents(const std::atomic<bool>& stop_token = false) const;
	Folder(Folder&& folder) noexcept = default;
	Folder(const Folder& folder) noexcept = default;
	Folder& operator=(const Folder& folder) noexcept = default;
	Folder& operator=(Folder&& folder) noexcept = default;
	Folder(const std::filesystem::path& path)
	{
		m_path_ = path;
	}
};

