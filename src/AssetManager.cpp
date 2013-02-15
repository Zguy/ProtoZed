/*
Copyright (c) 2013 Johannes Häggqvist

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include <ProtoZed/AssetManager.h>

#include <ProtoZed/Archive.h>
#include <ProtoZed/Log.h>

#include <vector>

namespace PZ
{
	typedef std::vector<std::pair<Path, Archive*>> ArchiveList;
	typedef std::map<Path, std::pair<Asset*, Archive*>> FileIndex;
	typedef std::map<std::string, AssetType> TypeMap;

	typedef std::map<std::string, AssetGroup> GroupMap;
	typedef std::map<Path, bool> GroupQueue;

	class AssetManager::Impl
	{
	public:
		Impl(Application *application) : application(application)
		{}

		void indexArchive(Archive *archive, bool onlyIndexRegisteredTypes)
		{
			FileList list;
			archive->GetAllFiles(list);

			for (FileList::iterator it = list.begin(); it != list.end(); ++it)
			{
				if (!onlyIndexRegisteredTypes || (onlyIndexRegisteredTypes && !getType(*it).empty()))
				{
					addFile((*it), archive);
				}
			}
		}

		bool addFile(const Path &filename, Archive *archive)
		{
			Asset *asset = nullptr;
			return fileIndex.insert(std::make_pair(filename, std::make_pair(asset, archive))).second;
		}
		FileIndex::iterator getFile(const Path &filename)
		{
			return fileIndex.find(filename);
		}

		bool loadFile(FileIndex::iterator fileIt, const AssetType &resType)
		{
			Archive *archive = nullptr;

			if (fileIt == fileIndex.end())
			{
				return false;
			}
			else
			{
				if ((*fileIt).second.first != nullptr)
				{
					return true;
				}
				else
				{
					archive = (*fileIt).second.second;
				}
			}

			const Path &filename = (*fileIt).first;

			if (resType.empty())
			{
				Log::Error("ProtoZed", "No asset type is registered for \""+filename.ToString()+"\"");
			}
			else
			{
				Asset *asset = assetFactory.Create(resType);
				if (asset == nullptr)
				{
					Log::Error("ProtoZed", "Asset \""+resType+"\" does not exist");
				}
				else
				{
					DataChunk data = archive->Get(filename);

					asset->application = application;
					if (asset->load(data))
					{
						(*fileIt).second.first = asset;

						return true;
					}
					else
					{
						Log::Error("ProtoZed", "Asset \""+resType+"\" failed to load \""+filename.ToString()+"\"");

						delete asset;
					}
				}
			}

			return false;
		}
		bool unloadFile(FileIndex::iterator fileIt)
		{
			if (fileIt != fileIndex.end())
			{
				if (isLoaded(fileIt))
				{
					Asset *&asset = (*fileIt).second.first;
					if (!asset->unload())
					{
						Log::Warning("ProtoZed", "The file \""+(*fileIt).first.ToString()+" failed to unload");
					}

					delete asset;
					asset = nullptr;
				}

				return true;
			}

			return false;
		}

		bool isLoaded(FileIndex::iterator fileIt)
		{
			return ((*fileIt).second.first != nullptr);
		}

		const AssetType &getType(const Path &filename) const
		{
			const std::string &fname = filename.ToString();
			std::string extension = fname.substr(fname.find_last_of('.')+1);
			TypeMap::const_iterator it = typeMap.find(extension);
			if (it != typeMap.cend())
			{
				return (*it).second;
			}
			else
			{
				return nullType;
			}
		}
		const AssetType &getType(FileIndex::iterator fileIt)
		{
			return getType((*fileIt).first);
		}

		Application *application;

		ArchiveList archives;
		FileIndex fileIndex;

		TypeMap typeMap;

		GroupMap groups;
		GroupQueue groupQueue;

		static const AssetType nullType;

		AssetManager::ArchiveFactory archiveFactory;
		AssetManager::AssetFactory assetFactory;
	};

	const AssetType AssetManager::Impl::nullType = "";

	AssetManager::AssetManager(Application &application) : p(new Impl(&application))
	{
	}
	AssetManager::~AssetManager()
	{
		for (FileIndex::iterator it = p->fileIndex.begin(); it != p->fileIndex.end(); ++it)
		{
			delete (*it).second.first;
			(*it).second.first  = nullptr;
			(*it).second.second = nullptr;
		}
		p->fileIndex.clear();

		for (ArchiveList::iterator it = p->archives.begin(); it != p->archives.end(); ++it)
		{
			delete (*it).second;
			(*it).second = nullptr;
		}
		p->archives.clear();

		delete p;
	}

	void AssetManager::SetType(const std::string &extension, const AssetType &type)
	{
		p->typeMap[extension] = type;
	}

	bool AssetManager::AddArchive(const Path &filename, const ArchiveType &type, bool indexAll, bool onlyIndexRegisteredTypes)
	{
		Archive *archive = p->archiveFactory.Create(type);
		if (archive == nullptr)
		{
			Log::Error("ProtoZed", "No archive registered for \""+type+"\"");
		}
		else if (archive->Open(filename))
		{
			p->archives.push_back(std::make_pair(filename, archive));

			if (indexAll)
			{
				p->indexArchive(archive, onlyIndexRegisteredTypes);
			}

			return true;
		}
		else
		{
			Log::Error("ProtoZed", "Could not open archive \""+filename.ToString()+"\"");
			delete archive;
		}

		return false;
	}
	bool AssetManager::RemoveArchive(const Path &filename)
	{
		for (ArchiveList::iterator it = p->archives.begin(); it != p->archives.end(); ++it)
		{
			if ((*it).first == filename)
			{
				Archive *archive = (*it).second;

				for (FileIndex::iterator it2 = p->fileIndex.begin(); it2 != p->fileIndex.end();)
				{
					if ((*it2).second.second == archive)
					{
						it2 = p->fileIndex.erase(it2);
					}
					else
					{
						++it2;
					}
				}

				delete archive;
				p->archives.erase(it);

				return true;
			}
		}

		return false;
	}

	bool AssetManager::AddGroup(const AssetGroup &group)
	{
		return p->groups.insert(std::make_pair(group.name, group)).second;
	}
	bool AssetManager::HasGroup(const std::string &name)
	{
		return (p->groups.find(name) != p->groups.end());
	}
	bool AssetManager::RemoveGroup(const std::string &name)
	{
		GroupMap::iterator it = p->groups.find(name);
		if (it != p->groups.end())
		{
			p->groups.erase(it);
			return true;
		}

		return false;
	}

	void AssetManager::IndexAll(bool onlyIndexRegisteredTypes)
	{
		for (ArchiveList::iterator it = p->archives.begin(); it != p->archives.end(); ++it)
		{
			p->indexArchive((*it).second, onlyIndexRegisteredTypes);
		}
	}
	bool AssetManager::IndexFile(const Path &filename)
	{
		for (ArchiveList::iterator it = p->archives.begin(); it != p->archives.end(); ++it)
		{
			Archive *archive = (*it).second;
			if (archive->Has(filename))
			{
				if (!p->addFile(filename, archive))
				{
					Log::Warning("ProtoZed", "The file \""+filename.ToString()+"\" is already indexed");
					return false;
				}
				else
				{
					return true;
				}
			}
		}

		Log::Error("ProtoZed", "Tried to index file \""+filename.ToString()+"\", but no archive contains it");
		return false;
	}

	void AssetManager::LoadAll()
	{
		for (FileIndex::iterator it = p->fileIndex.begin(); it != p->fileIndex.end(); ++it)
		{
			p->loadFile(it, p->getType(it));
		}
	}
	void AssetManager::UnloadAll()
	{
		for (FileIndex::iterator it = p->fileIndex.begin(); it != p->fileIndex.end(); ++it)
		{
			p->unloadFile(it);
		}
	}
	bool AssetManager::Load(const Path &filename)
	{
		FileIndex::iterator fileIt = p->getFile(filename);
		if (fileIt == p->fileIndex.end())
		{
			Log::Error("ProtoZed", "The file \""+filename.ToString()+"\" could not be loaded, because it's not indexed.");
		}
		else
		{
			return p->loadFile(fileIt, p->getType(fileIt));
		}

		return false;
	}
	bool AssetManager::LoadAs(const Path &filename, const AssetType &type)
	{
		FileIndex::iterator fileIt = p->getFile(filename);
		if (fileIt == p->fileIndex.end())
		{
			Log::Error("ProtoZed", "The file \""+filename.ToString()+"\" could not be loaded, because it's not indexed.");
		}
		else
		{
			return p->loadFile(fileIt, type);
		}

		return false;
	}
	bool AssetManager::Unload(const Path &filename)
	{
		FileIndex::iterator fileIt = p->getFile(filename);
		if (fileIt == p->fileIndex.end())
		{
			Log::Error("ProtoZed", "The file \""+filename.ToString()+"\" could not be unloaded, because it's not indexed");
		}
		else
		{
			return p->unloadFile(fileIt);
		}

		return false;
	}

	bool AssetManager::LoadGroupDirect(const std::string &name)
	{
		GroupMap::iterator it = p->groups.find(name);
		if (it != p->groups.end())
		{
			AssetGroup &group = (*it).second;

			for (FileList::iterator it = group.files.begin(); it != group.files.end(); ++it)
			{
				Load(*it);
			}

			return true;
		}

		return false;
	}
	bool AssetManager::UnloadGroupDirect(const std::string &name)
	{
		GroupMap::iterator it = p->groups.find(name);
		if (it != p->groups.end())
		{
			AssetGroup &group = (*it).second;

			for (FileList::iterator it = group.files.begin(); it != group.files.end(); ++it)
			{
				Unload(*it);
			}

			return true;
		}

		return false;
	}
	bool AssetManager::LoadGroup(const std::string &name)
	{
		GroupMap::iterator it = p->groups.find(name);
		if (it != p->groups.end())
		{
			AssetGroup &group = (*it).second;
			for (FileList::iterator it = group.files.begin(); it != group.files.end(); ++it)
			{
				p->groupQueue[(*it)] = true;
			}
			return true;
		}

		return false;
	}
	bool AssetManager::UnloadGroup(const std::string &name)
	{
		GroupMap::iterator it = p->groups.find(name);
		if (it != p->groups.end())
		{
			AssetGroup &group = (*it).second;
			for (FileList::iterator it = group.files.begin(); it != group.files.end(); ++it)
			{
				p->groupQueue[(*it)] = false;
			}
			return true;
		}

		return false;
	}
	void AssetManager::RunGroupQueue()
	{
		for (GroupQueue::const_iterator it = p->groupQueue.cbegin(); it != p->groupQueue.cend(); ++it)
		{
			const Path &file = (*it).first;
			bool load = (*it).second;

			if (load)
			{
				Load(file);
			}
			else
			{
				Unload(file);
			}
		}
		p->groupQueue.clear();
	}

	const Asset *AssetManager::Get(const Path &filename, bool autoLoad)
	{
		FileIndex::iterator fileIt = p->getFile(filename);
		if (fileIt == p->fileIndex.end())
		{
			Log::Warning("ProtoZed", "The file \""+filename.ToString()+"\" is not indexed, returning null asset");
			return nullptr;
		}
		else
		{
			Asset *&asset = (*fileIt).second.first;
			if (asset == nullptr)
			{
				if (!(autoLoad && p->loadFile(fileIt, p->getType(fileIt))))
				{
					Log::Warning("ProtoZed", "\""+filename.ToString()+"\" is not loaded, returning null asset");
					return nullptr;
				}
			}

			return asset;
		}
	}

	AssetManager::ArchiveFactory &AssetManager::getArchiveFactory() const
	{
		return p->archiveFactory;
	}
	AssetManager::AssetFactory &AssetManager::getAssetFactory() const
	{
		return p->assetFactory;
	}
}
