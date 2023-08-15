#pragma once
#include "FileSystemNode.hpp"

class Folder final : FileSystemNode
{
public:
	template<typename T, typename = std::enable_if<std::is_convertible_v<T, std::filesystem::path>>>
	explicit Folder(T&& path) : FileSystemNode(std::forward<T>(path)) {}
	~Folder() override = default;
	[[nodiscard]] int64_t get_size(const std::atomic<bool>& p_continue) const override;
	[[nodiscard]] std::vector<std::unique_ptr<FileSystemNode>> get_all_contents(const std::atomic<bool>& stop_token);
	Folder(Folder&& folder) noexcept;
	Folder(const Folder& folder) noexcept;
	Folder& operator=(const Folder& folder) noexcept;
	Folder& operator=(Folder&& folder) noexcept;
};

