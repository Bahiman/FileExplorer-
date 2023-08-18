#pragma once
#include <future>

#include"../headers/FileSystemNode.hpp"


class File final : public FileSystemNode
{
public:
	~File() override = default;
	[[nodiscard]] uint64_t get_size(const std::atomic<bool>& p_continue) const override;
	template<typename T, typename = std::enable_if<std::is_convertible_v<T, Path>>>
	explicit File(T&& path) : FileSystemNode(std::forward<T>(path)) {}
	File(const File& file) noexcept = default;
	File(File&& file) noexcept = default;
	File& operator=(const File& file) noexcept = default;
};
