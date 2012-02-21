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

#include <vector>

namespace PZ
{
	typedef std::vector<std::pair<std::string, Archive*>> ArchiveList;

	class ResourceManager::Impl
	{
	public:
		ArchiveList archives;
		ResourceManager::ArchiveFactory archiveFactory;

		ResourceManager::ResourceFactory resourceFactory;
	};

	ResourceManager::ResourceManager() : p(new Impl)
	{
	}
	ResourceManager::~ResourceManager()
	{
		for (ArchiveList::iterator it = p->archives.begin(); it != p->archives.end(); ++it)
		{
			delete (*it).second;
		}
		p->archives.clear();

		delete p;
	}

	bool ResourceManager::AddArchive(const std::string &filename, const std::string &type)
	{
		Archive *archive = p->archiveFactory.Create(type);
		if (archive != nullptr && archive->Open(filename))
		{
			p->archives.push_back(std::make_pair(filename, archive));
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
				delete (*it).second;
				p->archives.erase(it);
				return true;
			}
		}

		return false;
	}


	bool ResourceManager::LoadAll()
	{
		return false;
	}
	bool ResourceManager::Load(const std::string &filename)
	{
		Archive *archive = nullptr;
		for (ArchiveList::const_iterator it = p->archives.cbegin(); it != p->archives.cend(); ++it)
		{
			if ((*it).second->Has(filename))
			{
				archive = (*it).second;
				break;
			}
		}

		if (archive != nullptr)
		{
			DataChunk data = archive->Get(filename);

			Resource *resource = p->resourceFactory.Create("???");
			if (resource != nullptr)
			{
				resource->filename = filename;
				resource->load(data);

				return true;
			}
		}

		return false;
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
