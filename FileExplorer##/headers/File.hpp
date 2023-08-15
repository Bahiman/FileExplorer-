#pragma once
#include"../headers/FileSystemNode.hpp"

class File final : public FileSystemNode
{
public:
	~File() override = default;
	[[nodiscard]] int64_t get_size(const std::atomic<bool>& p_continue) const override;
	template<typename T, typename = std::enable_if<std::is_convertible_v<T, std::filesystem::path>>>
	explicit File(T&& path) : FileSystemNode(std::forward<T>(path)) {}
	File(const File& file) noexcept;
	File(File&& file) noexcept;
	File& operator=(const File& file) noexcept;
};