#pragma once
#include <filesystem>
#include <QFileInfo>
#include <memory>
#include <vector>

// A type alias for the Path
using Path = std::filesystem::path;

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
	[[nodiscard]] const std::filesystem::path& get_path() const;
	// a method for getting the size(implementation will be different depending on whether the node is a directory or a file)
	[[nodiscard]] virtual uint64_t get_size(const std::atomic<bool>& p_continue) const = 0;
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



