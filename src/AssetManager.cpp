#include "ProtoZed/AssetManager.h"

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

namespace PZ
{
	//TODO: Make these dynamic
	template<>
	std::string AssetManager<sf::Image>::GetDataFolder()
	{
		return "data/images/";
	}
	template<>
	std::string AssetManager<sf::Font>::GetDataFolder()
	{
		return "data/fonts/";
	}
	template<>
	std::string AssetManager<sf::SoundBuffer>::GetDataFolder()
	{
		return "data/sounds/";
	}

	// sf::Image
	/*template<>
	bool AssetManager<sf::Image>::PreloadAsset(const std::string &filename)
	{
		if (assets.find(filename) == assets.end())
		{
			sf::Image image;
			if (image.LoadFromFile(GetDataFolder()+filename))
			{
				assets[filename] = image;
			}
			else
			{
				return false;
			}
		}
		return true;
	}*/

	// sf::Font
	/*template<>
	bool AssetManager<sf::Font>::PreloadAsset(const std::string &filename)
	{
		if (assets.find(filename) == assets.end())
		{
			sf::Font font;
			if (font.LoadFromFile(GetDataFolder()+filename))
			{
				assets[filename] = font;
			}
			else
			{
				return false;
			}
		}
		return true;
	}*/

	// sf::SoundBuffer
	/*template<>
	bool AssetManager<sf::SoundBuffer>::PreloadAsset(const std::string &filename)
	{
		if (assets.find(filename) == assets.end())
		{
			sf::SoundBuffer soundBuffer;
			if (soundBuffer.LoadFromFile(GetDataFolder()+filename))
			{
				assets[filename] = soundBuffer;
			}
			else
			{
				return false;
			}
		}
		return true;
	}*/
}