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
#include "ProtoZed/AssetStorage.h"

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

namespace PZ
{
	// sf::Image
	template<>
	bool AssetStorage<sf::Image>::PreloadAsset(const std::string &filename)
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
	}

	// sf::Font
	template<>
	bool AssetStorage<sf::Font>::PreloadAsset(const std::string &filename)
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
	}

	// sf::SoundBuffer
	template<>
	bool AssetStorage<sf::SoundBuffer>::PreloadAsset(const std::string &filename)
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
	}
}