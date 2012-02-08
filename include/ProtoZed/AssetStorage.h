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
#ifndef PZ_AssetStorage_h__
#define PZ_AssetStorage_h__

#include <unordered_map>
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
		typedef std::unordered_map<std::string, T> AssetMap;
		AssetMap assets;
		static T nullAsset;
	};

	template<typename T>
	void AssetStorage<T>::SetDataFolder(const std::string &folder)
	{
		char last = folder.at(folder.size()-1);
		if (last != '/' && last != '\\')
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
			return assets.at(filename);
		else
			return nullAsset;
	}
}

#endif // PZ_AssetStorage_h__