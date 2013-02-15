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
#ifndef PZ_System_h__
#define PZ_System_h__

#include <ProtoZed/EventHandler.h>

#include <boost/noncopyable.hpp>

#include <string>

namespace PZ
{
	class Application;

	typedef std::string SystemType;

	/**
	 * \brief	A system.
	 * 				
	 * This is something that will run
	 * every frame and do some work.
	 */
	class System : public boost::noncopyable, public EventHandler
	{
	public:
		System(const SystemType &type, Application &application) : type(type), started(false), application(application)
		{}
		virtual ~System()
		{}

		/**
		 * \brief	Starts the system.
		 *
		 * \return	true if it succeeds, false if it fails.
		 */
		bool Start()
		{
			if (!started && OnStart())
			{
				started = true;
				return true;
			}
			else
			{
				return false;
			}
		}

		/**
		 * \brief	Stops the system.
		 *
		 * \return	true if it succeeds, false if it fails.
		 */
		bool Stop()
		{
			if (started && OnStop())
			{
				started = false;
				return true;
			}
			else
			{
				return false;
			}
		}

		virtual bool OnStart()
		{
			return true;
		}
		virtual bool OnStop()
		{
			return true;
		}

		/**
		 * \brief	Runs an update.
		 *
		 * \param	deltaTime	Time since the last frame.
		 */
		virtual void Update(float deltaTime) = 0;

		/**
		 * \brief	Gets the type.
		 *
		 * \return	The type.
		 */
		inline const SystemType &GetType() const { return type; }

		inline bool IsStarted() const { return started; }

	protected:
		inline Application &GetApplication() const
		{
			return application;
		}

	private:
		SystemType type;
		bool started;

		Application &application;
	};
}

#endif // PZ_System_h__
