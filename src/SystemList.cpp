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
#include <ProtoZed/SystemList.h>

#include <ProtoZed/Log.h>
#include <ProtoZed/Profiler.h>

#include <vector>

namespace PZ
{
	typedef std::vector<System*> SystemStore;

	class SystemList::Impl
	{
	public:
		SystemStore::iterator findSystem(const SystemType &type)
		{
			for (SystemStore::iterator it = systems.begin(); it != systems.end(); ++it)
			{
				if ((*it)->GetType() == type)
				{
					return it;
				}
			}

			return systems.end();
		}

		SystemStore systems;
	};

	SystemList::SystemList(Application &application) : p(new Impl), application(application)
	{
	}
	SystemList::~SystemList()
	{
		RemoveAll();

		delete p;
	}

	bool SystemList::Remove(const SystemType &type)
	{
		SystemStore::iterator it = p->findSystem(type);
		if (it != p->systems.end())
		{
			System *system (*it);

			// Warn if system not stopped
			if (system->IsStarted())
			{
				Log::Warning("ProtoZed", "Removing system \""+system->GetType()+"\" without stopping it");
			}

			delete system;

			p->systems.erase(it);

			return true;
		}
		else
		{
			return false;
		}
	}
	void SystemList::RemoveAll()
	{
		for (SystemStore::reverse_iterator it = p->systems.rbegin(); it != p->systems.rend(); ++it)
		{
			System *&system = (*it);

			// Warn if system not stopped
			if (system->IsStarted())
			{
				Log::Warning("ProtoZed", "Removing system \""+system->GetType()+"\" without stopping it");
			}

			delete system;
			system = nullptr;
		}
		p->systems.clear();
	}

	System *SystemList::Get(const SystemType &type) const
	{
		//Profile profile("GetSystem");

		System *system = nullptr;

		SystemStore::iterator it = p->findSystem(type);
		if (it != p->systems.end())
		{
			system = (*it);
		}

		return system;
	}

	void SystemList::StartAll()
	{
		for (SystemStore::iterator it = p->systems.begin(); it != p->systems.end(); ++it)
		{
			System *system = (*it);
			if (!system->IsStarted())
			{
				system->Start();
			}
		}
	}
	void SystemList::StopAll()
	{
		for (SystemStore::reverse_iterator it = p->systems.rbegin(); it != p->systems.rend(); ++it)
		{
			System *system = (*it);
			if (system->IsStarted())
			{
				(*it)->Stop();
			}
		}
	}
	void SystemList::UpdateAll(float deltaTime)
	{
		Profile profile("Systems");

		for (SystemStore::iterator it = p->systems.begin(); it != p->systems.end(); ++it)
		{
			System *system = (*it);
			if (system->IsStarted())
			{
				Profile profile(system->GetType());

				system->Update(deltaTime);
			}
		}
	}

	bool SystemList::AddImpl(const SystemType &type, System *system)
	{
		if (Has(type))
		{
			return false;
		}
		else
		{
			p->systems.push_back(system);

			return true;
		}
	}
	bool SystemList::InsertAfterImpl(const SystemType &type, const SystemType &after, System *system)
	{
		if (Has(type))
		{
			return false;
		}
		else
		{
			SystemStore::iterator it = p->findSystem(after);
			if (it != p->systems.end() && ++it != p->systems.end())
			{
				p->systems.insert(it, system);
			}
			else
			{
				// If "after" was not found, insert at the end
				p->systems.push_back(system);
			}

			return true;
		}
	}
	bool SystemList::InsertBeforeImpl(const SystemType &type, const SystemType &before, System *system)
	{
		if (Has(type))
		{
			return false;
		}
		else
		{
			SystemStore::iterator it = p->findSystem(before);
			if (it != p->systems.end())
			{
				p->systems.insert(it, system);
			}
			else
			{
				// If "before" was not found, insert at the end
				p->systems.push_back(system);
			}

			return true;
		}
	}
}