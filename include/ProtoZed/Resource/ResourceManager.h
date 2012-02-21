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
#ifndef PZ_ResourceManager_h__
#define PZ_ResourceManager_h__

#include <ProtoZed/ObjectFactory/ObjectFactory.h>
#include <ProtoZed/Archive.h>
#include <ProtoZed/ArchiveType.h>
#include <ProtoZed/Resource/Resource.h>

#include <string>

namespace PZ
{
	class ResourceManager
	{
	public:
		ResourceManager();
		~ResourceManager();

		template<class T>
		bool RegisterArchiveHandler(const ArchiveType &type)
		{
			return getArchiveFactory().Register<T>(type);
		}
		bool UnregisterArchiveHandler(const ArchiveType &type)
		{
			return getArchiveFactory().Unregister(type);
		}

		template<class T>
		bool RegisterResourceType(const std::string &type)
		{
			return getResourceFactory<T>(type);
		}
		bool UnregisterResourceType(const std::string &type)
		{
			return getResourceFactory().Unregister(type);
		}

		bool AddArchive(const std::string &filename, const ArchiveType &type);
		bool RemoveArchive(const std::string &filename);

		bool LoadAll();
		bool Load(const std::string &filename);
		bool Unload(const std::string &filename);
		Resource &Get(const std::string &filename);

	private:
		class Impl;
		Impl *p;

		typedef ::ObjectFactory<Archive*(), ArchiveType> ArchiveFactory;
		ArchiveFactory &getArchiveFactory() const;

		typedef ::ObjectFactory<Resource*(), std::string> ResourceFactory;
		ResourceFactory &getResourceFactory() const;
	};
}

#endif // PZ_ResourceManager_h__
