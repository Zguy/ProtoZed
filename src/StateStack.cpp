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
#include <ProtoZed/StateStack.h>

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

	class StateStack::Impl
	{
	public:
		typedef std::stack<State*> StateStack;
		typedef std::map<std::string, State*> StateMap;
		typedef std::queue<Todo::Entry> TodoQueue;

		Impl(Application *application) : application(application)
		{}

		State *getNewStateByName(const std::string &stateName)
		{
			return appStateFactory.Create(stateName, *application);
		}

		void change(const std::string &stateName, StringMap &options)
		{
			Log::Info("ProtoZed", "Changing state to \""+stateName+"\"");
			State *state = getNewStateByName(stateName);
			if (state != nullptr)
			{
				stateStack.top()->Stop();
				delete stateStack.top();
				stateStack.pop();

				stateStack.push(state);

				stateStack.top()->Start(options);

				Log::Info("ProtoZed", "Changed state to \""+stateName+"\"");
			}
			else
			{
				Log::Error("ProtoZed", "State \""+stateName+"\" was not found");
			}
		}

		void push(const std::string &stateName, StringMap &options)
		{
			Log::Info("ProtoZed", "Pushing state \""+stateName+"\"");
			State *state = getNewStateByName(stateName);
			if (state != nullptr)
			{
				if (!stateStack.empty())
					stateStack.top()->Pause();

				stateStack.push(state);

				stateStack.top()->Start(options);

				Log::Info("ProtoZed", "Pushed state \""+stateName+"\"");
			}
			else
			{
				Log::Error("ProtoZed", "State \""+stateName+"\" was not found");
			}
		}

		void pop()
		{
			if (!stateStack.empty())
			{
				Log::Info("ProtoZed", "Popping state");

				stateStack.top()->Stop();

				delete stateStack.top();
				stateStack.pop();

				if (!stateStack.empty())
					stateStack.top()->Resume();

				Log::Info("ProtoZed", "Popped state");
			}
		}

		void popAll()
		{
			if (!stateStack.empty())
			{
				Log::Info("ProtoZed", "Popping all states");

				while (!stateStack.empty())
				{
					if (stateStack.top()->IsStarted())
					{
						stateStack.top()->Stop();
					}
					delete stateStack.top();
					stateStack.pop();
				}

				Log::Info("ProtoZed", "Popped all states");
			}
		}

		Application *application;

		StateStack stateStack;
		TodoQueue  todoQueue;
		PZ::StateStack::AppStateFactory appStateFactory;
	};

	StateStack::StateStack(Application &application) : p(new Impl(&application))
	{
	}
	StateStack::~StateStack()
	{
		p->popAll();

		delete p;
	}

	void StateStack::Update()
	{
		while (!p->todoQueue.empty())
		{
			Todo::Entry &entry = p->todoQueue.front();
			switch (entry.type)
			{
			case Todo::CHANGE  : p->change(entry.stateName, entry.options); break;
			case Todo::PUSH    : p->push(entry.stateName, entry.options);   break;
			case Todo::POP     : p->pop();                                  break;
			case Todo::POP_ALL : p->popAll();                               break;
			}
			p->todoQueue.pop();
		}
	}

	void StateStack::Change(const std::string &stateName)
	{
		Todo::Entry todo;
		todo.type      = Todo::CHANGE;
		todo.stateName = stateName;
		p->todoQueue.push(todo);
	}
	void StateStack::Change(const std::string &stateName, StringMap &options)
	{
		Todo::Entry todo;
		todo.type      = Todo::CHANGE;
		todo.stateName = stateName;
		todo.options   = options;
		p->todoQueue.push(todo);
	}

	void StateStack::Push(const std::string &stateName)
	{
		Todo::Entry todo;
		todo.type      = Todo::PUSH;
		todo.stateName = stateName;
		p->todoQueue.push(todo);
	}
	void StateStack::Push(const std::string &stateName, StringMap &options)
	{
		Todo::Entry todo;
		todo.type      = Todo::PUSH;
		todo.stateName = stateName;
		todo.options   = options;
		p->todoQueue.push(todo);
	}

	void StateStack::Pop()
	{
		Todo::Entry todo;
		todo.type      = Todo::POP;
		todo.stateName = "";
		p->todoQueue.push(todo);
	}

	void StateStack::PopAll()
	{
		Todo::Entry todo;
		todo.type      = Todo::POP_ALL;
		todo.stateName = "";
		p->todoQueue.push(todo);
	}

	bool StateStack::IsEmpty()
	{
		return p->stateStack.empty();
	}

	State *StateStack::GetCurrent() const
	{
		if (!p->stateStack.empty())
			return p->stateStack.top();
		else
			return nullptr;
	}

	StateStack::AppStateFactory &StateStack::getAppStateFactory() const
	{
		return p->appStateFactory;
	}
}