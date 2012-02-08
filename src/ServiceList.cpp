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
#include <ProtoZed/ServiceList.h>

#include <ProtoZed/Log.h>
#include <ProtoZed/Profiler.h>

#include <vector>

namespace PZ
{
	typedef std::vector<Service*> ServiceStore;

	class ServiceList::Impl
	{
	public:
		ServiceStore::iterator findService(const ServiceType &type)
		{
			for (ServiceStore::iterator it = services.begin(); it != services.end(); ++it)
			{
				if ((*it)->GetType() == type)
				{
					return it;
				}
			}

			return services.end();
		}

		ServiceStore services;
	};

	ServiceList::ServiceList(Application &application) : p(new Impl), application(application)
	{
	}
	ServiceList::~ServiceList()
	{
		RemoveAll();

		delete p;
	}

	Service *ServiceList::Add(const ServiceType &type)
	{
		if (Has(type))
		{
			return nullptr;
		}
		else
		{
			Service *service = factory.Create(type, type, application);
			if (service != nullptr)
			{
				p->services.push_back(service);
			}

			return service;
		}
	}
	Service *ServiceList::InsertAfter(const ServiceType &type, const ServiceType &after)
	{
		if (Has(type))
		{
			return nullptr;
		}
		else
		{
			Service *service = factory.Create(type, type, application);
			if (service != nullptr)
			{
				ServiceStore::const_iterator it = p->findService(after);
				if (it != p->services.end() && ++it != p->services.end())
				{
					p->services.insert(it, service);
				}
				else
				{
					// If "after" was not found, insert at the end
					p->services.push_back(service);
				}
			}

			return service;
		}
	}
	Service *ServiceList::InsertBefore(const ServiceType &type, const ServiceType &before)
	{
		if (Has(type))
		{
			return nullptr;
		}
		else
		{
			Service *service = factory.Create(type, type, application);
			if (service != nullptr)
			{
				ServiceStore::const_iterator it = p->findService(before);
				if (it != p->services.end())
				{
					p->services.insert(it, service);
				}
				else
				{
					// If "before" was not found, insert at the end
					p->services.push_back(service);
				}
			}

			return service;
		}
	}

	bool ServiceList::Remove(const ServiceType &type)
	{
		ServiceStore::const_iterator it = p->findService(type);
		if (it != p->services.end())
		{
			// Warn if service not stopped
			if ((*it)->IsStarted())
			{
				Log::Warning("ProtoZed", "Removing service \""+(*it)->GetType()+"\" without stopping it");
			}

			delete (*it);

			p->services.erase(it);

			return true;
		}
		else
		{
			return false;
		}
	}
	void ServiceList::RemoveAll()
	{
		for (ServiceStore::reverse_iterator it = p->services.rbegin(); it != p->services.rend(); ++it)
		{
			// Warn if service not stopped
			if ((*it)->IsStarted())
			{
				Log::Warning("ProtoZed", "Removing service \""+(*it)->GetType()+"\" without stopping it");
			}

			delete (*it);
		}
		p->services.clear();
	}

	Service *ServiceList::Get(const ServiceType &type) const
	{
		Service *service = nullptr;

		ServiceStore::iterator it = p->findService(type);
		if (it != p->services.end())
		{
			service = (*it);
		}

		return service;
	}

	void ServiceList::StartAll()
	{
		for (ServiceStore::iterator it = p->services.begin(); it != p->services.end(); ++it)
		{
			(*it)->Start();
		}
	}
	void ServiceList::StopAll()
	{
		for (ServiceStore::reverse_iterator it = p->services.rbegin(); it != p->services.rend(); ++it)
		{
			(*it)->Stop();
		}
	}
	void ServiceList::UpdateAll(float deltaTime)
	{
		Profile profile("Services");

		for (ServiceStore::iterator it = p->services.begin(); it != p->services.end(); ++it)
		{
			if ((*it)->IsStarted())
			{
				Profile profile((*it)->GetType());

				(*it)->Update(deltaTime);
			}
		}
	}

	bool ServiceList::hasType(const ServiceType &type) const
	{
		for (ServiceFactory::ConstIterator it = factory.GetBegin(); it != factory.GetEnd(); ++it)
		{
			if ((*it).first == type)
			{
				return true;
			}
		}

		return false;
	}
}