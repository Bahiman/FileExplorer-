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
	File(File&& file) noexcept = default;
	File(const File& file, bool d) noexcept
	{
		this->m_path_ = file.m_path_;
	}
	File& operator=(const File& file) noexcept = default;
	File& operator=(File&& file) noexcept = default;
};
