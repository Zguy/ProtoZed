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
#ifndef PZ_VideoMode_h__
#define PZ_VideoMode_h__

#include <ProtoZed/Vector2.h>

namespace PZ
{
	class VideoMode
	{
	public:
		VideoMode();
		VideoMode(const Vector2i &windowResolution, const Vector2i &virtualResolution, bool fullscreen = false);
		VideoMode(int windowWidth, int windowHeight, int virtualWidth, int virtualHeight, bool fullscreen = false);
		~VideoMode();

		void SetVirtualResolution(const Vector2i &resolution);
		void SetVirtualResolution(int width, int height);
		void SetWindowResolution(const Vector2i &resolution, bool sameVirtual = true);
		void SetWindowResolution(int width, int height, bool sameVirtual = true);
		void SetWindowResolution(const Vector2i &resolution, bool fullscreen, bool sameVirtual = true);
		void SetWindowResolution(int width, int height, bool fullscreen, bool sameVirtual = true);
		void SetFullscreen(bool fullscreen);

		inline const Vector2i &GetVirtualResolution() const { return virtualResolution; }
		inline const Vector2i &GetWindowResolution() const { return windowResolution; }
		inline bool GetFullscreen() const { return windowFullscreen; }

	private:
		Vector2i virtualResolution;
		Vector2i windowResolution;
		bool windowFullscreen;
	};
}

#endif // PZ_VideoMode_h__