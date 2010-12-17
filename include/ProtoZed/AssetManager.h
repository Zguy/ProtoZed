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
#ifndef AssetManager_h__
#define AssetManager_h__

#include <map>
#include <string>

namespace PZ
{
	template<typename T>
	class AssetManager
	{
	public:
		AssetManager()
		{}
		~AssetManager()
		{
			while (!assets.empty())
			{
				UnloadAsset((*assets.begin()).first);
			}
		}

		bool PreloadAsset(const std::string &filename);
		void UnloadAsset(const std::string &filename);
		const T &GetAsset(const std::string &filename);

	private:
		std::string GetDataFolder();

		typedef std::map<std::string, T> AssetMap;
		AssetMap assets;
		T nullAsset;
	};

	template<typename T>
	bool AssetManager<T>::PreloadAsset(const std::string &filename)
	{
		if (assets.find(filename) == assets.end())
		{
			T asset;
			if (asset.LoadFromFile(GetDataFolder()+filename))
				assets[filename] = asset;
			else
				return false;
		}
		return true;
	}

	template<typename T>
	void AssetManager<T>::UnloadAsset(const std::string &filename)
	{
		assets.erase(filename);
	}

	template<typename T>
	const T &AssetManager<T>::GetAsset(const std::string &filename)
	{
		if (PreloadAsset(filename))
			return assets[filename];
		else
			return nullAsset;
	}
}

#endif // AssetManager_h__