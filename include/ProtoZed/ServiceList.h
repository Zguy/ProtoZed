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
#ifndef PZ_ServiceList_h__
#define PZ_ServiceList_h__

#include <ProtoZed/NonCopyable.h>
#include <ProtoZed/Service.h>

namespace PZ
{
	class ServiceList : public NonCopyable
	{
	public:
		ServiceList(Application &application);
		~ServiceList();

		template<class T>
		T *Add(const ServiceType &type)
		{
			T *service = new T(type, application);
			if (AddImpl(type, service))
			{
				return service;
			}
			else
			{
				delete service;
				return nullptr;
			}
		}
		template<class T>
		T *InsertAfter(const ServiceType &type, const ServiceType &after)
		{
			T *service = new T(type, application);
			if (InsertAfterImpl(type, after, service))
			{
				return service;
			}
			else
			{
				delete service;
				return nullptr;
			}
		}
		template<class T>
		T *InsertBefore(const ServiceType &type, const ServiceType &before)
		{
			T *service = new T(type, application);
			if (InsertBeforeImpl(type, before, service))
			{
				return service;
			}
			else
			{
				delete service;
				return nullptr;
			}
		}

		bool Remove(const ServiceType &type);
		void RemoveAll();
		
		inline bool Has(const ServiceType &type) const
		{
			return (Get(type) != nullptr);
		}
		
		template<class T>
		T *Get(const ServiceType &type) const
		{
			return dynamic_cast<T*>(Get(type));
		}
		Service *Get(const ServiceType &type) const;

		void StartAll();
		void StopAll();

		void UpdateAll(float deltaTime);

	private:
		bool AddImpl(const ServiceType &type, Service *service);
		bool InsertAfterImpl(const ServiceType &type, const ServiceType &after, Service *service);
		bool InsertBeforeImpl(const ServiceType &type, const ServiceType &before, Service *service);

		class Impl;
		Impl *p;

		Application &application;
	};
}

#endif // PZ_ServiceList_h__