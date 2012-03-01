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

	bool ServiceList::Remove(const ServiceType &type)
	{
		ServiceStore::iterator it = p->findService(type);
		if (it != p->services.end())
		{
			Service *service (*it);

			// Warn if service not stopped
			if (service->IsStarted())
			{
				Log::Warning("ProtoZed", "Removing service \""+service->GetType()+"\" without stopping it");
			}

			delete service;

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
			Service *&service = (*it);

			// Warn if service not stopped
			if (service->IsStarted())
			{
				Log::Warning("ProtoZed", "Removing service \""+service->GetType()+"\" without stopping it");
			}

			delete service;
			service = nullptr;
		}
		p->services.clear();
	}

	Service *ServiceList::Get(const ServiceType &type) const
	{
		Profile profile("GetService");

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
			Service *service = (*it);
			if (!service->IsStarted())
			{
				service->Start();
			}
		}
	}
	void ServiceList::StopAll()
	{
		for (ServiceStore::reverse_iterator it = p->services.rbegin(); it != p->services.rend(); ++it)
		{
			Service *service = (*it);
			if (service->IsStarted())
			{
				(*it)->Stop();
			}
		}
	}
	void ServiceList::UpdateAll(float deltaTime)
	{
		Profile profile("Services");

		for (ServiceStore::iterator it = p->services.begin(); it != p->services.end(); ++it)
		{
			Service *service = (*it);
			if (service->IsStarted())
			{
				Profile profile(service->GetType());

				service->Update(deltaTime);
			}
		}
	}

	bool ServiceList::AddImpl(const ServiceType &type, Service *service)
	{
		if (Has(type))
		{
			return false;
		}
		else
		{
			p->services.push_back(service);

			return true;
		}
	}
	bool ServiceList::InsertAfterImpl(const ServiceType &type, const ServiceType &after, Service *service)
	{
		if (Has(type))
		{
			return false;
		}
		else
		{
			ServiceStore::iterator it = p->findService(after);
			if (it != p->services.end() && ++it != p->services.end())
			{
				p->services.insert(it, service);
			}
			else
			{
				// If "after" was not found, insert at the end
				p->services.push_back(service);
			}

			return true;
		}
	}
	bool ServiceList::InsertBeforeImpl(const ServiceType &type, const ServiceType &before, Service *service)
	{
		if (Has(type))
		{
			return false;
		}
		else
		{
			ServiceStore::iterator it = p->findService(before);
			if (it != p->services.end())
			{
				p->services.insert(it, service);
			}
			else
			{
				// If "before" was not found, insert at the end
				p->services.push_back(service);
			}

			return true;
		}
	}
}