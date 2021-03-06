#include "Refs.hpp"
#include "Index.hpp"
#include "Util.hpp"
#include "FileSystem.hpp"

#include <iostream>

namespace Sit {
namespace Index {

Index index;

void Index::Load()
{
	try {
		if (FileSystem::IsExist(FileSystem::REPO_ROOT / FileSystem::SIT_ROOT / "index")) {
			boost::filesystem::ifstream indexFile(FileSystem::REPO_ROOT / FileSystem::SIT_ROOT / "index");
			unsigned fileCount = 0;
			indexFile >> fileCount;
			indexFile.get();
			for (unsigned i = 0; i < fileCount; ++i) {
				std::string fileName;
				std::string sha1Value;
				indexFile >> sha1Value;
				indexFile.get();
				std::getline(indexFile, fileName);
				_index.insert(std::make_pair(boost::filesystem::path(fileName), sha1Value));
			}
			indexFile.close();
		}
	} catch (const boost::filesystem::filesystem_error &fe) {
		std::cerr << fe.what() << std::endl;
	} catch (const std::exception &ec) {
		std::cerr << ec.what() << std::endl;
	}
}

void Index::Save() const
{
	try {
		boost::filesystem::ofstream indexFile(FileSystem::REPO_ROOT / FileSystem::SIT_ROOT / "index");
		indexFile << _index.size() << std::endl;
		for (auto &element : _index) {
			indexFile << element.second << " " << element.first.generic_string() << std::endl;
		}
		indexFile.close();
	} catch (const boost::filesystem::filesystem_error &fe) {
		std::cerr << fe.what() << std::endl;
	} catch (const std::exception &ec) {
		std::cerr << ec.what() << std::endl;
	}
}

void Index::Clear()
{
	_index.clear();
}

void IndexBase::Insert(const boost::filesystem::path &file, const std::string &content)
{
	try {
		_index[file] = content;
	} catch (const std::exception &ec) {
		std::cerr << ec.what() << std::endl;
	}
}

unsigned IndexBase::Remove(const boost::filesystem::path &path)
{
	if (path.empty()) {
		_index.clear();
	}
	unsigned rmCount = 0;
	try {
		auto newPath = path.relative_path();
		std::vector<boost::filesystem::path> wouldRm;
		for (auto &element : _index) {
			if (element.first.generic_string() == newPath.generic_string() || 
					(element.first.generic_string().find(newPath.generic_string()) == 0 && 
					element.first.generic_string().length() > newPath.generic_string().length() && 
					element.first.generic_string().at(newPath.generic_string().length()) == '/')
				) {
				wouldRm.push_back(element.first);
				++rmCount;
			}
		}
		for (auto &element : wouldRm) {
			_index.erase(element);
		}
	} catch (const std::exception &ec) {
		std::cerr << ec.what() << std::endl;
	}
	return rmCount;
}

bool IndexBase::InIndex(const boost::filesystem::path& path) const
{
	try {
		if (path.generic_string().back() != '/') {
			return _index.count(path) > 0;
		} else {
			for (const auto &element : _index) {
				auto filename = element.first;
				if (filename.generic_string().find(path.generic_string()) == 0) {
					return true;
				}
			}
			return false;
		}
	} catch (const std::exception &ec) {
		std::cerr << ec.what() << std::endl;
	}
	return false;
}

const std::string& IndexBase::GetID(const boost::filesystem::path &path) const
{
	const auto iter = _index.find(path);
	if (iter != _index.end())
		return iter->second;
	throw Util::SitException(std::string("Path ") + path.string() + " not in the index.");
}

const FileSet& IndexBase::GetIndex() const
{
	return _index;
}

FileSet IndexBase::ListFile(const std::string &prefix) const
{
	FileSet result;
	if (prefix == "") {
		return _index;
	}
	for (const auto &element : this->_index) {
		if (element.first.generic_string() == prefix) {
			result.insert(element);
		} else if (element.first.generic_string().find(prefix) == 0 && element.first.generic_string().at(prefix.length()) == '/') {
			result.insert(element);
		} else if (element.first.generic_string().find(prefix) == 0 && prefix.back() == '/') {
			result.insert(element);
		}
	}
	return result;
}

void CommitIndex::flattenTree(const Objects::Tree &tree, const boost::filesystem::path& prefix)
{
	for (auto &item : tree) {
		if (item.type == Objects::BLOB) {
			_index.insert(std::make_pair(prefix / item.filename, item.id));
		} else {
			const Objects::Tree subtree(Objects::GetTree(item.id));
			flattenTree(subtree, prefix / item.filename);
		}
	}
}

void CommitIndex::load(const std::string& id)
{
	if (id == Refs::EMPTY_REF) {
		return ;
	}
	const Objects::Commit commit(Objects::GetCommit(id));
	const Objects::Tree tree(Objects::GetTree(commit.tree));
	flattenTree(tree, "");
}

void WorkingIndex::load()
{
	const auto ls(FileSystem::ListRecursive(FileSystem::REPO_ROOT));
	for (const auto &path : ls) {
		if (FileSystem::IsDirectory(path)) continue;
		const std::string sha1(FileSystem::FileSHA1(path));
		_index.insert(std::make_pair(path, sha1));
	}
}

IndexBase GetIndex(const std::string &id)
{
	if (id == "index")
		return index;
	if (id == "master")
		return CommitIndex(Refs::Get(Refs::Local("master")));
	if (id == "HEAD")
		return CommitIndex(Refs::Get("HEAD"));
	if (id == "work")
		return WorkingIndex();
	return CommitIndex(id);
}

}
}

