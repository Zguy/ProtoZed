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

#include <boost/noncopyable.hpp>

#include <string>

namespace PZ
{
	class Application;

	typedef std::string AssetType;

	class AssetGroup
	{
	public:
		AssetGroup()
		{}
		AssetGroup(const std::string &name) : name(name)
		{}
		~AssetGroup()
		{}

		std::string name;
		FileList files;
	};

	class AssetManager : public boost::noncopyable
	{
	public:
		AssetManager(Application &application);
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

		bool AddGroup(const AssetGroup &group);
		bool HasGroup(const std::string &name);
		bool RemoveGroup(const std::string &name);

		void IndexAll(bool onlyIndexRegisteredTypes = true);
		bool IndexFile(const Path &filename);

		void LoadAll();
		void UnloadAll();
		bool Load(const Path &filename);
		bool LoadAs(const Path &filename, const AssetType &type);
		bool Unload(const Path &filename);

		bool LoadGroupDirect(const std::string &name);
		bool UnloadGroupDirect(const std::string &name);

		/**
		 * \brief	Places the files of a group in the load queue.
		 *
		 * If some files is already in the unload queue,
		 * they will be removed.
		 * 
		 * \param	name	The name of the group.
		 *
		 * \return	true if the group exists.
		 */
		bool LoadGroup(const std::string &name);

		/**
		 * \brief	Places the files of a group in the unload queue.
		 * 
		 * If some files is already in the load queue,
		 * they will be removed.
		 *
		 * \param	name	The name.
		 *
		 * \return	true if it succeeds, false if it fails.
		 */
		bool UnloadGroup(const std::string &name);

		/**
		 * \brief	Loads and unloads the files in the group queue.
		 */
		void RunGroupQueue();

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
