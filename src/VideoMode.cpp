/*
Copyright (c) 2013 Johannes Häggqvist

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
#include <ProtoZed/VideoMode.h>

namespace PZ
{
	VideoMode::VideoMode() : virtualResolution(0,0), windowResolution(0,0), windowFullscreen(false)
	{
	}
	VideoMode::VideoMode(const Vector2i &windowResolution, const Vector2i &virtualResolution, bool fullscreen) : virtualResolution(virtualResolution), windowResolution(windowResolution), windowFullscreen(fullscreen)
	{
	}
	VideoMode::VideoMode(int windowWidth, int windowHeight, int virtualWidth, int virtualHeight, bool fullscreen) : virtualResolution(virtualWidth, virtualHeight), windowResolution(windowWidth, windowHeight), windowFullscreen(fullscreen)
	{
	}
	VideoMode::~VideoMode()
	{
	}

	void VideoMode::SetVirtualResolution(const Vector2i &resolution)
	{
		virtualResolution = resolution;
	}
	void VideoMode::SetVirtualResolution(int width, int height)
	{
		virtualResolution.x = width;
		virtualResolution.y = height;
	}
	void VideoMode::SetWindowResolution(const Vector2i &resolution, bool sameVirtual)
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
	void VideoMode::SetWindowResolution(const Vector2i &resolution, bool fullscreen, bool sameVirtual)
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
}