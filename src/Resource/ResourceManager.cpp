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
#include <ProtoZed/Resource/ResourceManager.h>

#include <ProtoZed/Archive.h>
#include <ProtoZed/Log.h>

#include <vector>

namespace PZ
{
	typedef std::vector<std::pair<std::string, Archive*>> ArchiveList;
	typedef std::map<std::string, std::pair<Resource*, Archive*>> FileIndex;
	typedef std::map<std::string, ResourceType> TypeMap;

	class ResourceManager::Impl
	{
	public:
		void indexArchive(Archive *archive)
		{
			FileList list;
			archive->GetAllFiles(list);

			for (FileList::iterator it = list.begin(); it != list.end(); ++it)
			{
				addFile((*it), archive);
			}
		}

		void addFile(const std::string &filename, Archive *archive)
		{
			fileIndex.insert(std::make_pair(filename, std::make_pair<Resource*, Archive*>(nullptr, archive)));
		}
		FileIndex::iterator getFile(const std::string &filename)
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

			const std::string &filename = (*fileIt).first;

			DataChunk data = archive->Get(filename);

			ResourceType resType = getType(filename);
			if (resType.empty())
			{
				Log::Error("ProtoZed", "No ResourceType is registered for \""+filename+"\"");
			}
			else
			{
				Resource *resource = resourceFactory.Create(resType);
				if (resource == nullptr)
				{
					Log::Error("ProtoZed", "Resource \""+resType+"\" does not exist");
				}
				else
				{
					resource->filename = filename;
					if (resource->load(data))
					{
						(*fileIt).second.first = resource;

						return true;
					}
					else
					{
						Log::Error("ProtoZed", "Resource \""+resType+"\" failed to load \""+filename+"\"");

						delete resource;
					}
				}
			}

			return false;
		}
		bool unloadFile(FileIndex::iterator fileIt)
		{
			if (fileIt != fileIndex.end())
			{
				delete (*fileIt).second.first;
				(*fileIt).second.first = nullptr;

				return true;
			}

			return false;
		}

		const ResourceType &getType(const std::string &filename) const
		{
			std::string extension = filename.substr(filename.find_last_of('.')+1);
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

		static const ResourceType nullType;
		static const NullResource nullResource;

		ResourceManager::ArchiveFactory archiveFactory;
		ResourceManager::ResourceFactory resourceFactory;
	};

	const ResourceType ResourceManager::Impl::nullType = "";
	const NullResource ResourceManager::Impl::nullResource = NullResource();

	ResourceManager::ResourceManager() : p(new Impl)
	{
	}
	ResourceManager::~ResourceManager()
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

	void ResourceManager::SetType(const std::string &extension, const ResourceType &type)
	{
		p->typeMap[extension] = type;
	}

	bool ResourceManager::AddArchive(const std::string &filename, const ArchiveType &type, bool indexAll)
	{
		Archive *archive = p->archiveFactory.Create(type);
		if (archive != nullptr && archive->Open(filename))
		{
			p->archives.push_back(std::make_pair(filename, archive));

			if (indexAll)
			{
				p->indexArchive(archive);
			}

			return true;
		}
		else
		{
			delete archive;
		}

		return false;
	}
	bool ResourceManager::RemoveArchive(const std::string &filename)
	{
		for (ArchiveList::iterator it = p->archives.begin(); it != p->archives.end(); ++it)
		{
			if ((*it).first == filename)
			{
				//FIXME: Remove all files from fileIndex belonging to this archive

				delete (*it).second;
				p->archives.erase(it);

				return true;
			}
		}

		return false;
	}

	void ResourceManager::IndexAll()
	{
		for (ArchiveList::iterator it = p->archives.begin(); it != p->archives.end(); ++it)
		{
			p->indexArchive((*it).second);
		}
	}
	bool ResourceManager::IndexFile(const std::string &filename)
	{
		for (ArchiveList::iterator it = p->archives.begin(); it != p->archives.end(); ++it)
		{
			Archive *archive = (*it).second;
			if (archive->Has(filename))
			{
				p->addFile(filename, archive);

				return true;
			}
		}

		Log::Error("ProtoZed", "Tried to index file \""+filename+"\", but no archive contains it");
		return false;
	}

	void ResourceManager::LoadAll()
	{
		for (FileIndex::iterator it = p->fileIndex.begin(); it != p->fileIndex.end(); ++it)
		{
			p->loadFile(it);
		}
	}
	void ResourceManager::UnloadAll()
	{
		for (FileIndex::iterator it = p->fileIndex.begin(); it != p->fileIndex.end(); ++it)
		{
			p->unloadFile(it);
		}
	}
	bool ResourceManager::Load(const std::string &filename)
	{
		FileIndex::iterator fileIt = p->getFile(filename);
		if (fileIt == p->fileIndex.end())
		{
			Log::Error("ProtoZed", "The file \""+filename+"\" could not be loaded, because it's not indexed.");
		}
		else
		{
			return p->loadFile(fileIt);
		}

		return false;
	}
	bool ResourceManager::Unload(const std::string &filename)
	{
		FileIndex::iterator fileIt = p->getFile(filename);
		if (fileIt == p->fileIndex.end())
		{
			Log::Error("ProtoZed", "The file \""+filename+"\" could not be unloaded, because it's not indexed");
		}
		else
		{
			return p->unloadFile(fileIt);
		}

		return false;
	}

	const Resource &ResourceManager::Get(const std::string &filename, bool autoLoad)
	{
		FileIndex::iterator fileIt = p->getFile(filename);
		if (fileIt == p->fileIndex.end())
		{
			Log::Error("ProtoZed", "The file \""+filename+"\" is not indexed");
			return p->nullResource;
		}
		else
		{
			Resource *resource = (*fileIt).second.first;
			if (!resource->IsLoaded())
			{
				if (!(autoLoad && p->loadFile(fileIt)))
				{
					Log::Warning("ProtoZed", "Returning unloaded resource");
				}
			}

			return *resource;
		}
	}

	ResourceManager::ArchiveFactory &ResourceManager::getArchiveFactory() const
	{
		return p->archiveFactory;
	}
	ResourceManager::ResourceFactory &ResourceManager::getResourceFactory() const
	{
		return p->resourceFactory;
	}
}
