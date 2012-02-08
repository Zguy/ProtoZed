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
#include <ProtoZed/AppStateManager.h>

#include <ProtoZed/Log.h>

#include <stack>
#include <queue>
#include <map>

namespace PZ
{
	namespace Todo
	{
		enum Type
		{
			CHANGE,
			PUSH,
			POP,
			POP_ALL
		};
		struct Entry
		{
			Type type;
			std::string stateName;
			StringMap options;
		};
	}

	typedef std::stack<AppState*> StateStack;
	typedef std::map<std::string, AppState*> StateMap;
	typedef std::queue<Todo::Entry> TodoQueue;

	class AppStateManager::Impl
	{
	public:
		Impl(Application &application) : application(application)
		{}

		AppState *getNewStateByName(const std::string &stateName)
		{
			return appStateFactory.Create(stateName, application);
		}

		void changeState(const std::string &stateName, StringMap &options)
		{
			Log::Info("ProtoZed", "Changing state to \""+stateName+"\"");
			AppState *state = getNewStateByName(stateName);
			if (state != nullptr)
			{
				stateStack.top()->Stop();
				stateStack.top()->UnloadAssets();
				delete stateStack.top();
				stateStack.pop();

				stateStack.push(state);

				stateStack.top()->LoadAssets();
				stateStack.top()->Start(options);

				Log::Info("ProtoZed", "Changed state to \""+stateName+"\"");
			}
			else
			{
				Log::Info("ProtoZed", "State \""+stateName+"\" was not found");
			}
		}

		void pushState(const std::string &stateName, StringMap &options)
		{
			Log::Info("ProtoZed", "Pushing state \""+stateName+"\"");
			AppState *state = getNewStateByName(stateName);
			if (state != nullptr)
			{
				if (!stateStack.empty())
					stateStack.top()->Pause();

				stateStack.push(state);

				stateStack.top()->LoadAssets();
				stateStack.top()->Start(options);

				Log::Info("ProtoZed", "Pushed state \""+stateName+"\"");
			}
			else
			{
				Log::Info("ProtoZed", "State \""+stateName+"\" was not found");
			}
		}

		void popState()
		{
			if (!stateStack.empty())
			{
				Log::Info("ProtoZed", "Popping state");

				stateStack.top()->Stop();
				stateStack.top()->UnloadAssets();

				delete stateStack.top();
				stateStack.pop();

				if (!stateStack.empty())
					stateStack.top()->Resume();

				Log::Info("ProtoZed", "Popped state");
			}
		}

		void popAllStates()
		{
			if (!stateStack.empty())
			{
				Log::Info("ProtoZed", "Popping all states");

				while (!stateStack.empty())
				{
					if (stateStack.top()->IsStarted())
					{
						stateStack.top()->Stop();
						stateStack.top()->UnloadAssets();
					}
					delete stateStack.top();
					stateStack.pop();
				}

				Log::Info("ProtoZed", "Popped all states");
			}
		}

		Application &application;

		StateStack stateStack;
		TodoQueue  todoQueue;
		AppStateManager::AppStateFactory appStateFactory;
	};

	AppStateManager::AppStateManager(Application &application) : p(new Impl(application))
	{
	}
	AppStateManager::~AppStateManager()
	{
		p->popAllStates();

		delete p;
	}

	void AppStateManager::Update()
	{
		while (!p->todoQueue.empty())
		{
			Todo::Entry &entry = p->todoQueue.front();
			switch (entry.type)
			{
			case Todo::CHANGE  : p->changeState(entry.stateName, entry.options); break;
			case Todo::PUSH    : p->pushState(entry.stateName, entry.options);   break;
			case Todo::POP     : p->popState();                                  break;
			case Todo::POP_ALL : p->popAllStates();                              break;
			}
			p->todoQueue.pop();
		}
	}

	bool AppStateManager::UnregisterState(const std::string &stateName)
	{
		return p->appStateFactory.Unregister(stateName);
	}

	void AppStateManager::ChangeState(const std::string &stateName)
	{
		Todo::Entry todo;
		todo.type      = Todo::CHANGE;
		todo.stateName = stateName;
		p->todoQueue.push(todo);
	}
	void AppStateManager::ChangeState(const std::string &stateName, StringMap &options)
	{
		Todo::Entry todo;
		todo.type      = Todo::CHANGE;
		todo.stateName = stateName;
		todo.options   = options;
		p->todoQueue.push(todo);
	}

	void AppStateManager::PushState(const std::string &stateName)
	{
		Todo::Entry todo;
		todo.type      = Todo::PUSH;
		todo.stateName = stateName;
		p->todoQueue.push(todo);
	}
	void AppStateManager::PushState(const std::string &stateName, StringMap &options)
	{
		Todo::Entry todo;
		todo.type      = Todo::PUSH;
		todo.stateName = stateName;
		todo.options   = options;
		p->todoQueue.push(todo);
	}

	void AppStateManager::PopState()
	{
		Todo::Entry todo;
		todo.type      = Todo::POP;
		todo.stateName = "";
		p->todoQueue.push(todo);
	}

	void AppStateManager::PopAllStates()
	{
		Todo::Entry todo;
		todo.type      = Todo::POP_ALL;
		todo.stateName = "";
		p->todoQueue.push(todo);
	}

	bool AppStateManager::IsEmpty()
	{
		return p->stateStack.empty();
	}

	AppState *AppStateManager::GetCurrentState() const
	{
		if (!p->stateStack.empty())
			return p->stateStack.top();
		else
			return nullptr;
	}

	AppStateManager::AppStateFactory &AppStateManager::getAppStateFactory() const
	{
		return p->appStateFactory;
	}
}