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
#include <ProtoZed/AppStateManager.h>

#include <ProtoZed/Application.h>

#include <stack>
#include <queue>
#include <map>

namespace PZ
{
	namespace Todo
	{
		enum Type
		{
			CHANGE = 1,
			PUSH,
			POP,
			POP_ALL
		};
		struct Entry
		{
			Type type;
			std::string stateName;
			StringMap *options;
		};
	}

	typedef std::stack<AppState*> StateStack;
	typedef std::map<std::string, AppState*> StateMap;
	typedef std::queue<Todo::Entry> TodoQueue;

	class AppStateManagerImpl
	{
	public:
		AppState *getNewStateByName(const std::string &stateName)
		{
			return appStateFactory.Create(stateName);
		}

		void changeState(const std::string &stateName, StringMap *const options)
		{
			Application::GetSingleton().GetLogManager().GetLog("ProtoZed").Info(Log::LVL_LOW, "Changing state to \""+stateName+"\"");
			AppState *state = getNewStateByName(stateName);
			if (state != NULL)
			{
				stateStack.top()->Stop();
				stateStack.top()->UnloadAssets();
				delete stateStack.top();
				stateStack.pop();

				stateStack.push(state);

				stateStack.top()->LoadAssets();
				stateStack.top()->Start(options);

				Application::GetSingleton().GetLogManager().GetLog("ProtoZed").Info(Log::LVL_LOW, "Changed state to \""+stateName+"\"");
			}
			else
			{
				Application::GetSingleton().GetLogManager().GetLog("ProtoZed").Info(Log::LVL_LOW, "State \""+stateName+"\" was not found");
			}
		}

		void pushState(const std::string &stateName, StringMap *const options)
		{
			Application::GetSingleton().GetLogManager().GetLog("ProtoZed").Info(Log::LVL_LOW, "Pushing state \""+stateName+"\"");
			AppState *state = getNewStateByName(stateName);
			if (state != NULL)
			{
				if (!stateStack.empty())
					stateStack.top()->Pause();

				stateStack.push(state);

				stateStack.top()->LoadAssets();
				stateStack.top()->Start(options);

				Application::GetSingleton().GetLogManager().GetLog("ProtoZed").Info(Log::LVL_LOW, "Pushed state \""+stateName+"\"");
			}
			else
			{
				Application::GetSingleton().GetLogManager().GetLog("ProtoZed").Info(Log::LVL_LOW, "State \""+stateName+"\" was not found");
			}
		}

		void popState()
		{
			if (!stateStack.empty())
			{
				Application::GetSingleton().GetLogManager().GetLog("ProtoZed").Info(Log::LVL_LOW, "Popping state");

				stateStack.top()->Stop();
				stateStack.top()->UnloadAssets();

				delete stateStack.top();
				stateStack.pop();

				if (!stateStack.empty())
					stateStack.top()->Resume();

				Application::GetSingleton().GetLogManager().GetLog("ProtoZed").Info(Log::LVL_LOW, "Popped state");
			}
		}

		void popAllStates()
		{
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
		}

		StateStack stateStack;
		TodoQueue  todoQueue;
		AppStateManager::AppStateFactory appStateFactory;
	};

	AppStateManager::AppStateManager() : p(new AppStateManagerImpl)
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
			if (entry.options != NULL)
				delete entry.options;
			p->todoQueue.pop();
		}
	}

	bool AppStateManager::UnregisterState(const std::string &stateName)
	{
		return p->appStateFactory.Unregister(stateName);
	}

	void AppStateManager::ChangeState(const std::string &stateName, StringMap *options)
	{
		Todo::Entry todo;
		todo.type      = Todo::CHANGE;
		todo.stateName = stateName;
		todo.options   = options;
		p->todoQueue.push(todo);
	}

	void AppStateManager::PushState(const std::string &stateName, StringMap *options)
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
		todo.options   = NULL;
		p->todoQueue.push(todo);
	}

	void AppStateManager::PopAllStates()
	{
		Todo::Entry todo;
		todo.type      = Todo::POP_ALL;
		todo.stateName = "";
		todo.options   = NULL;
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
			return NULL;
	}

	AppStateManager::AppStateFactory &AppStateManager::getAppStateFactory() const
	{
		return p->appStateFactory;
	}
}