#pragma once
#include <filesystem>
#include <QFileInfo>
#include <memory>
#include <vector>

// A type alias for the Path
using Path = std::wstring;

// A type alias for the directory iterator 
using FSDirIt = std::filesystem::directory_iterator;

class FileSystemNode
{
public:
	// Deleting the default constructor so that the child classes can't accidentally call it forgetting to pass the path
	FileSystemNode() = default;
	virtual ~FileSystemNode() = default;
	template<typename T, typename = std::enable_if<std::is_convertible_v<T, Path>>>
	explicit FileSystemNode(T&& path);
	// Member functions that will return the data about the node using `QFileInfo`
	[[nodiscard]] std::string get_creation_date(const Path& p_path) const;
	[[nodiscard]] std::string get_last_access_date(const Path& p_path) const;
	[[nodiscard]] std::string get_last_modification_date(const Path& p_path) const;
	[[nodiscard]] const Path& get_path() const;
	[[nodiscard]] std::wstring_view get_file_name() const;
	template<typename T, typename = std::enable_if<std::is_convertible_v<T, Path>>>
	void set_path(T&& path)
	{
		m_path_ = std::forward<T>(path);
	}
	[[nodiscard]] Path& get_modifiable_path()
	{
		return m_path_;

	}
	// a method for getting the size(implementation will be different depending on whether the node is a directory or a file)
	[[nodiscard]] virtual uint64_t get_size(const std::atomic<bool>& p_continue = false) const = 0;
	[[nodiscard]] Path&& get_moveable_path()
	{
		return std::move(m_path_);
	}

	FileSystemNode(FileSystemNode&& filesystemnode) noexcept = default;
	FileSystemNode(const FileSystemNode& filesystemnode) noexcept = default;
	FileSystemNode& operator=(const FileSystemNode& filesystemnode) noexcept = default;
	FileSystemNode& operator=(FileSystemNode&& filesystemnode) noexcept = default;
protected:
	// the path of the node
	Path m_path_;
};

// a type alias for a vector of unique pointers to `FileSystemNode`s

using FSNodes = std::vector<std::unique_ptr<FileSystemNode>>;


template <typename T, typename>
FileSystemNode::FileSystemNode(T&& path)
{
	m_path_ = std::forward<T>(path);
}



