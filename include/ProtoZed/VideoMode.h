/*
	Copyright 2010-2011 Johannes Häggqvist

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
#ifndef VideoMode_h__
#define VideoMode_h__

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/View.hpp>

namespace PZ
{
	class VideoMode
	{
	public:
		VideoMode();
		VideoMode(const sf::Vector2i &windowResolution, const sf::Vector2i &virtualResolution, bool fullscreen = false);
		VideoMode(int windowWidth, int windowHeight, int virtualWidth, int virtualHeight, bool fullscreen = false);
		~VideoMode();

		void SetVirtualResolution(const sf::Vector2i &resolution);
		void SetVirtualResolution(int width, int height);
		void SetWindowResolution(const sf::Vector2i &resolution, bool sameVirtual = true);
		void SetWindowResolution(int width, int height, bool sameVirtual = true);
		void SetWindowResolution(const sf::Vector2i &resolution, bool fullscreen, bool sameVirtual = true);
		void SetWindowResolution(int width, int height, bool fullscreen, bool sameVirtual = true);
		void SetFullscreen(bool fullscreen);

		inline const sf::Vector2i &GetVirtualResolution() const { return virtualResolution; }
		inline const sf::Vector2i &GetWindowResolution() const { return windowResolution; }
		inline bool GetFullscreen() const { return windowFullscreen; }

		inline const sf::View &GetView() const { return view; }

	private:
		void calculateView();

		sf::View view;

		sf::Vector2i virtualResolution;
		sf::Vector2i windowResolution;
		bool windowFullscreen;
	};
}

#endif // VideoMode_h__