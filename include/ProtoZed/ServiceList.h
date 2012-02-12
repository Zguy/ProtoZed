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
#include <ProtoZed/ObjectFactory/ObjectFactory.h>

namespace PZ
{
	class ServiceList : public NonCopyable
	{
		friend class Application;
	public:
		ServiceList(Application &application);
		~ServiceList();

		template<class T>
		bool Register(const ServiceType &type)
		{
			if (hasType(type))
			{
				Unregister(type);
			}

			return factory.Register<T>(type);
		}
		bool Unregister(const ServiceType &type)
		{
			Remove(type);
			return factory.Unregister(type);
		}

		Service *Add(const ServiceType &type);
		Service *InsertAfter(const ServiceType &type, const ServiceType &after);
		Service *InsertBefore(const ServiceType &type, const ServiceType &before);

		template<class T>
		T *Add(const ServiceType &type)
		{
			return dynamic_cast<T*>(Add(type));
		}
		template<class T>
		T *InsertAfter(const ServiceType &type, const ServiceType &after)
		{
			return dynamic_cast<T*>(InsertAfter(type, after));
		}
		template<class T>
		T *InsertBefore(const ServiceType &type, const ServiceType &before)
		{
			return dynamic_cast<T*>(InsertBefore(type, before));
		}

		bool Remove(const ServiceType &type);
		void RemoveAll();
		
		inline bool Has(const ServiceType &type) const
		{
			return (Get(type) != nullptr);
		}
		Service *Get(const ServiceType &type) const;
		
		template<class T>
		T *Get(const ServiceType &type) const
		{
			return dynamic_cast<T*>(Get(type));
		}

		void StartAll();
		void StopAll();

	protected:
		void UpdateAll(float deltaTime);

	private:
		bool hasType(const ServiceType &type) const;

		class Impl;
		Impl *p;

		Application &application;

		typedef ObjectFactory<Service*(const ServiceType&, Application&), ServiceType> ServiceFactory;
		ServiceFactory factory;
	};
}

#endif // PZ_ServiceList_h__