/*
Copyright (c) 2012 Johannes Häggqvist

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

	class AssetManager::Impl
	{
	public:
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

		bool loadFile(FileIndex::iterator fileIt)
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

			AssetType resType = getType(filename);
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

					asset->filename = filename;
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
					if (!(*fileIt).second.first->unload())
					{
						Log::Warning("ProtoZed", "The file \""+(*fileIt).first.ToString()+" failed to unload");
					}

					delete (*fileIt).second.first;
					(*fileIt).second.first = nullptr;
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

		ArchiveList archives;
		FileIndex fileIndex;

		TypeMap typeMap;

		static const AssetType nullType;

		AssetManager::ArchiveFactory archiveFactory;
		AssetManager::AssetFactory assetFactory;
	};

	const AssetType AssetManager::Impl::nullType = "";

	AssetManager::AssetManager() : p(new Impl)
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
			p->loadFile(it);
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
			return p->loadFile(fileIt);
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
				if (!(autoLoad && p->loadFile(fileIt)))
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
