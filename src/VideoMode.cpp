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
#include <ProtoZed/VideoMode.h>

namespace PZ
{
	VideoMode::VideoMode() : virtualResolution(0,0), windowResolution(0,0), windowFullscreen(false)
	{
	}
	VideoMode::VideoMode(const sf::Vector2i &windowResolution, const sf::Vector2i &virtualResolution, bool fullscreen) : virtualResolution(virtualResolution), windowResolution(windowResolution), windowFullscreen(fullscreen)
	{
		calculateView();
	}
	VideoMode::VideoMode(int windowWidth, int windowHeight, int virtualWidth, int virtualHeight, bool fullscreen) : virtualResolution(virtualWidth, virtualHeight), windowResolution(windowWidth, windowHeight), windowFullscreen(fullscreen)
	{
		calculateView();
	}
	VideoMode::~VideoMode()
	{
	}

	void VideoMode::SetVirtualResolution(const sf::Vector2i &resolution)
	{
		virtualResolution = resolution;
		calculateView();
	}
	void VideoMode::SetVirtualResolution(int width, int height)
	{
		virtualResolution.x = width;
		virtualResolution.y = height;
		calculateView();
	}
	void VideoMode::SetWindowResolution(const sf::Vector2i &resolution, bool sameVirtual)
	{
		windowResolution = resolution;

		if (sameVirtual)
			SetVirtualResolution(resolution);
	}
	void VideoMode::SetWindowResolution(int width, int height, bool sameVirtual)
	{
		windowResolution.x = width;
		windowResolution.y = height;

		if (sameVirtual)
			SetVirtualResolution(width, height);
	}
	void VideoMode::SetWindowResolution(const sf::Vector2i &resolution, bool fullscreen, bool sameVirtual)
	{
		windowResolution = resolution;
		windowFullscreen = fullscreen;

		if (sameVirtual)
			SetVirtualResolution(resolution);
	}
	void VideoMode::SetWindowResolution(int width, int height, bool fullscreen, bool sameVirtual)
	{
		windowResolution.x = width;
		windowResolution.y = height;
		windowFullscreen = fullscreen;

		if (sameVirtual)
			SetVirtualResolution(width, height);
	}
	void VideoMode::SetFullscreen(bool fullscreen)
	{
		windowFullscreen = fullscreen;
	}

	void VideoMode::calculateView()
	{
		view.SetFromRect(sf::FloatRect(0.f,0.f,static_cast<float>(virtualResolution.x),static_cast<float>(virtualResolution.y)));
	}
}