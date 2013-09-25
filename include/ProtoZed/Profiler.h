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
#ifndef PZ_Profiler_h__
#define PZ_Profiler_h__

#include <ProtoZed/Singleton.h>

#include <boost/noncopyable.hpp>

#include <string>

namespace PZ
{
	class Profiler : public Singleton<Profiler>
	{
	public:
		Profiler();
		~Profiler();

		void StartCapture();
		void StopCapture();
		bool IsCapturing() const;

		void Begin(const std::string &name);
		void End();

		void BeginFrame();
		void EndFrame();
		void NextFrame();

		void SetTargetFrameTime(float target);

		void WriteLog(const std::string &filename) const;

	private:
		class Impl;
		Impl *p;
	};

	class Profile : boost::noncopyable
	{
	public:
		Profile(const std::string &name);
		~Profile();
	};
}

#endif // PZ_Profiler_h__
