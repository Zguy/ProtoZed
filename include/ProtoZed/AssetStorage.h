/*
	Copyright 2010 Johannes Häggqvist

	This file is part of ProtoZed.

	ProtoZed is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	ProtoZed is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with ProtoZed.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef AssetStorage_h__
#define AssetStorage_h__

#include <map>
#include <string>

namespace PZ
{
	template<typename T>
	class AssetStorage
	{
	public:
		AssetStorage()
		{}
		~AssetStorage()
		{
			while (!assets.empty())
			{
				UnloadAsset((*assets.begin()).first);
			}
		}

		inline const std::string &GetDataFolder() const { return dataFolder; }
		void SetDataFolder(const std::string &folder);

		bool PreloadAsset(const std::string &filename);
		void UnloadAsset(const std::string &filename);
		const T &GetAsset(const std::string &filename);

	private:
		std::string dataFolder;
		typedef std::map<std::string, T> AssetMap;
		AssetMap assets;
		T nullAsset;
	};

	template<typename T>
	void PZ::AssetStorage<T>::SetDataFolder(const std::string &folder)
	{
		if (folder.back() != '/' && folder.back() != '\\')
			dataFolder = folder + '/';
		else
			dataFolder = folder;
	}

	template<typename T>
	void AssetStorage<T>::UnloadAsset(const std::string &filename)
	{
		assets.erase(filename);
	}

	template<typename T>
	const T &AssetStorage<T>::GetAsset(const std::string &filename)
	{
		if (PreloadAsset(filename))
			return assets[filename];
		else
			return nullAsset;
	}
}

#endif // AssetStorage_h__