#ifndef _SRC_INDEX_HPP_
#define _SRC_INDEX_HPP_

#include "FileSystem.hpp"
#include <map>

namespace Sit {
namespace Index {

class IndexBase {
protected:
	std::map<boost::filesystem::path, std::string> _index;
public:
	/**
	 * Check whether the `path` in the index or not
	 */
	bool InIndex(const boost::filesystem::path& path) const;

	/**
	 * Remove `path` from the index and its object file
	 */
	unsigned Remove(const boost::filesystem::path &path);

	/**
	 * Insert `file` and the sha1 of `content` to the index,
	 * and write `content` to the object file
	 */
	void Insert(const boost::filesystem::path & file, const std::string & content);

	/**
	 * Return the _index;
	 */
	const std::map<boost::filesystem::path, std::string>& GetIndex() const;
};

class Index : public IndexBase {
public:
	Index(): IndexBase() {}

	/**
	 * Load the index from `.sit/index`
	 */
	void Load();

	/**
	 * Save the index to `.sit/index`
	 */
	void Save() const;
};

class CommitIndex : public IndexBase {
	/**
	 * Load the file list from a commit
	 */
	void load(const std::string& id);

public:
	CommitIndex(const std::string& id): IndexBase() {}
};

extern Index index;

}
}
#endif