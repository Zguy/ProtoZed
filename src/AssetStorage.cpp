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
#include <ProtoZed/AssetStorage.h>

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

namespace PZ
{
	// sf::Image
	template<> sf::Image AssetStorage<sf::Image>::nullAsset = sf::Image();
	template<>
	bool AssetStorage<sf::Image>::PreloadAsset(const std::string &filename)
	{
		if (assets.find(filename) == assets.end())
		{
			auto result = assets.insert(std::make_pair(filename, sf::Image()));
			sf::Image &image = (*result.first).second;

			if (!image.LoadFromFile(GetDataFolder()+filename))
			{
				assets.erase(result.first);
				return false;
			}
		}
		return true;
	}

	// sf::Font
	template<> sf::Font AssetStorage<sf::Font>::nullAsset = sf::Font();
	template<>
	bool AssetStorage<sf::Font>::PreloadAsset(const std::string &filename)
	{
		if (assets.find(filename) == assets.end())
		{
			auto result = assets.insert(std::make_pair(filename, sf::Font()));
			sf::Font &font = (*result.first).second;

			if (!font.LoadFromFile(GetDataFolder()+filename))
			{
				assets.erase(result.first);
				return false;
			}
		}
		return true;
	}

	// sf::SoundBuffer
	template<> sf::SoundBuffer AssetStorage<sf::SoundBuffer>::nullAsset = sf::SoundBuffer();
	template<>
	bool AssetStorage<sf::SoundBuffer>::PreloadAsset(const std::string &filename)
	{
		if (assets.find(filename) == assets.end())
		{
			auto result = assets.insert(std::make_pair(filename, sf::SoundBuffer()));
			sf::SoundBuffer &soundBuffer = (*result.first).second;

			if (!soundBuffer.LoadFromFile(GetDataFolder()+filename))
			{
				assets.erase(result.first);
				return false;
			}
		}
		return true;
	}
}