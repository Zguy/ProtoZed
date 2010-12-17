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