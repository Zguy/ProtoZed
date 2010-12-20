/*
	Copyright 2010 Johannes Häggqvist

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
#include "ProtoZed/AppStateManager.h"

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
			}
		}

		void pushState(const std::string &stateName, StringMap *const options)
		{
			AppState *state = getNewStateByName(stateName);
			if (state != NULL)
			{
				if (!stateStack.empty())
					stateStack.top()->Pause();

				stateStack.push(state);

				stateStack.top()->LoadAssets();
				stateStack.top()->Start(options);
			}
		}

		void popState()
		{
			if (!stateStack.empty())
			{
				stateStack.top()->Stop();
				stateStack.top()->UnloadAssets();

				delete stateStack.top();
				stateStack.pop();

				if (!stateStack.empty())
					stateStack.top()->Resume();
			}
		}

		void popAllStack()
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
		//StateMap   states;
		TodoQueue  todoQueue;
		AppStateFactory appStateFactory;
	};

	AppStateManager::AppStateManager()
	{
		p = new AppStateManagerImpl;
	}

	AppStateManager::~AppStateManager()
	{
		p->popAllStack();
		//DeleteAllStates();

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
			case Todo::POP_ALL : p->popAllStack();                               break;
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

	/*void AppStateManager::AddState(const std::string &stateName, AppState *state)
	{
		if (p->states.find(stateName) == p->states.end())
			p->states[stateName] = state;
	}

	void AppStateManager::RemoveState(const std::string &stateName)
	{
		StateMap::iterator it = p->states.find(stateName);
		if (it != p->states.end())
		{
			delete (*it).second;
			p->states.erase(it);
		}
	}

	void AppStateManager::DeleteAllStates()
	{
		for (StateMap::iterator it = p->states.begin(); it != p->states.end(); ++it)
			delete (*it).second;
		p->states.clear();
	}

	AppState *AppStateManager::GetStateByName(const std::string &stateName)
	{
		StateMap::iterator it = p->states.find(stateName);
		if (it != p->states.end())
			return (*it).second;
		else
			return NULL;
	}

	StateList AppStateManager::GetAllStates()
	{
		StateList list;
		for (StateMap::iterator it = p->states.begin(); it != p->states.end(); ++it)
			list.push_back((*it).second);

		return list;
	}*/

	void AppStateManager::ChangeState(const std::string &stateName, StringMap *const options)
	{
		Todo::Entry todo;
		todo.type      = Todo::CHANGE;
		todo.stateName = stateName;
		todo.options   = options;
		p->todoQueue.push(todo);
	}

	void AppStateManager::PushState(const std::string &stateName, StringMap *const options)
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

	void AppStateManager::PopAllStack()
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

	AppState *AppStateManager::GetCurrentState()
	{
		if (!p->stateStack.empty())
			return p->stateStack.top();
		else
			return NULL;
	}

	AppStateFactory &AppStateManager::getAppStateFactory()
	{
		return p->appStateFactory;
	}
}