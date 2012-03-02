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
#ifndef PZ_AssetManager_h__
#define PZ_AssetManager_h__

#include <ProtoZed/ObjectFactory/ObjectFactory.h>
#include <ProtoZed/Path.h>
#include <ProtoZed/Archive.h>
#include <ProtoZed/Asset.h>

#include <string>

namespace PZ
{
	typedef std::string AssetType;

	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager();

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
		bool RegisterAssetType(const AssetType &type)
		{
			return getAssetFactory().Register<T>(type);
		}
		bool UnregisterAssetType(const AssetType &type)
		{
			return getAssetFactory().Unregister(type);
		}

		void SetType(const std::string &extension, const AssetType &type);

		bool AddArchive(const Path &filename, const ArchiveType &type, bool indexAll = true, bool onlyIndexRegisteredTypes = true);
		bool RemoveArchive(const Path &filename);

		void IndexAll(bool onlyIndexRegisteredTypes = true);
		bool IndexFile(const Path &filename);

		void LoadAll();
		void UnloadAll();
		bool Load(const Path &filename);
		bool Unload(const Path &filename);

		template<class T>
		const T *Get(const Path &filename, bool autoLoad = true)
		{
			return dynamic_cast<const T*>(Get(filename, autoLoad));
		}
		const Asset *Get(const Path &filename, bool autoLoad = true);

	private:
		class Impl;
		Impl *p;

		typedef ::ObjectFactory<Archive*(), ArchiveType> ArchiveFactory;
		ArchiveFactory &getArchiveFactory() const;

		typedef ::ObjectFactory<Asset*(), AssetType> AssetFactory;
		AssetFactory &getAssetFactory() const;
	};
}

#endif // PZ_AssetManager_h__
