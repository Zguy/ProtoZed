/*
Copyright (c) 2013 Johannes H�ggqvist

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
#ifndef PZ_StateStack_h__
#define PZ_StateStack_h__

#include <ProtoZed/State.h>
#include <ProtoZed/ObjectFactory/ObjectFactory.h>

#include <boost/noncopyable.hpp>

#include <vector>
#include <string>

namespace PZ
{
	class Application;

	class StateStack : boost::noncopyable
	{
	public:
		StateStack(Application &application);
		~StateStack();

		void Update();

		template<class T>
		bool Register(const std::string &stateName)
		{
			return getAppStateFactory().Register<T>(stateName);
		}
		bool Unregister(const std::string &stateName)
		{
			return getAppStateFactory().Unregister(stateName);
		}

		void Change(const std::string &stateName);
		void Change(const std::string &stateName, StringMap &options);
		void Push(const std::string &stateName);
		void Push(const std::string &stateName, StringMap &options);
		void Pop();
		void PopAll();

		bool IsEmpty();

		State *GetCurrent() const;

	private:
		class Impl;
		Impl *p;

		typedef ::ObjectFactory<State*(Application&), std::string> AppStateFactory;
		AppStateFactory &getAppStateFactory() const;
	};
}

#endif // PZ_StateStack_h__
