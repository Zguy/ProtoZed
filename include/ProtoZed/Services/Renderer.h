/*
Copyright (c) 2012 Johannes H�ggqvist

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
#ifndef PZ_Renderer_h__
#define PZ_Renderer_h__

#include <ProtoZed/Service.h>

#include <ProtoZed/VideoMode.h>

namespace PZ
{
	class Renderer : public Service
	{
	public:
		Renderer(const ServiceType &type, Application &application);
		virtual ~Renderer();

		virtual bool Start();
		virtual bool Stop();

		virtual void Update(float deltaTime) = 0;

		void SetVideoMode(const VideoMode &newVideoMode);
		inline const VideoMode &GetVideMode() const { return videoMode; }

		void SetTitle(const std::string &newTitle);
		inline const std::string &GetTitle() const { return title; }

	protected:
		virtual void videoModeUpdated()
		{}
		virtual bool canChangeTitle() const
		{
			return false;
		}

		VideoMode videoMode;
		std::string title;
	};
}

#endif // PZ_Renderer_h__