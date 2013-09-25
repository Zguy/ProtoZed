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
#ifndef PZ_State_h__
#define PZ_State_h__

#include <ProtoZed/EventHandler.h>

#include <boost/noncopyable.hpp>

#include <map>
#include <string>

namespace PZ
{
	typedef std::map<std::string, std::string> StringMap;

	class Application;

	class State : boost::noncopyable, public EventHandler
	{
	public:
		State(Application &application) : started(false), paused(false), application(application)
		{}
		virtual ~State()
		{}

		virtual void Update(float deltaTime) {}

		void Start(StringMap &options)
		{
			if (!started)
			{
				OnStart(options);
				started = true;
			}
		}
		void Stop()
		{
			if (started)
			{
				OnStop();
				started = false;
			}
		}
		void Pause()
		{
			if (!paused)
			{
				OnPause();
				paused = true;
			}
		}
		void Resume()
		{
			if (paused)
			{
				OnResume();
				paused = false;
			}
		}

		inline bool IsStarted() const { return started; }
		inline bool IsPaused() const { return paused; }

	protected:
		virtual void OnStart(StringMap &options) {}
		virtual void OnStop() {}

		virtual void OnPause() {}
		virtual void OnResume() {}

		inline Application &GetApplication() const
		{
			return application;
		}

	private:
		bool started;
		bool paused;

		Application &application;
	};
}

#endif // PZ_State_h__
