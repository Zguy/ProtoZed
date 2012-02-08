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
#ifndef PZ_AppStateManager_h__
#define PZ_AppStateManager_h__

#include <ProtoZed/AppState.h>
#include <ProtoZed/ObjectFactory/ObjectFactory.h>

#include <vector>
#include <string>

namespace PZ
{
	class Application;

	class AppStateManager
	{
	public:
		AppStateManager(Application &application);
		~AppStateManager();

		void Update();

		template<class T>
		bool RegisterState(const std::string &stateName)
		{
			return getAppStateFactory().Register<T>(stateName);
		}
		bool UnregisterState(const std::string &stateName);

		void ChangeState(const std::string &stateName);
		void ChangeState(const std::string &stateName, StringMap &options);
		void PushState(const std::string &stateName);
		void PushState(const std::string &stateName, StringMap &options);
		void PopState();
		void PopAllStates();

		bool IsEmpty();

		AppState *GetCurrentState() const;

	private:
		class Impl;
		Impl *p;

		typedef ::ObjectFactory<AppState*(Application&), std::string> AppStateFactory;
		AppStateFactory &getAppStateFactory() const;
	};
}

#endif // PZ_AppStateManager_h__