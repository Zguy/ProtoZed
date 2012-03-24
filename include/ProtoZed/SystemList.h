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
#ifndef PZ_SystemList_h__
#define PZ_SystemList_h__

#include <ProtoZed/NonCopyable.h>
#include <ProtoZed/System.h>

namespace PZ
{
	class SystemList : public NonCopyable
	{
	public:
		SystemList(Application &application);
		~SystemList();

		template<class T>
		T *Add(const SystemType &type)
		{
			T *system = new T(type, application);
			if (AddImpl(type, system))
			{
				return system;
			}
			else
			{
				delete system;
				return nullptr;
			}
		}
		template<class T>
		T *InsertAfter(const SystemType &type, const SystemType &after)
		{
			T *system = new T(type, application);
			if (InsertAfterImpl(type, after, system))
			{
				return system;
			}
			else
			{
				delete system;
				return nullptr;
			}
		}
		template<class T>
		T *InsertBefore(const SystemType &type, const SystemType &before)
		{
			T *system = new T(type, application);
			if (InsertBeforeImpl(type, before, system))
			{
				return system;
			}
			else
			{
				delete system;
				return nullptr;
			}
		}

		bool Remove(const SystemType &type);
		void RemoveAll();
		
		inline bool Has(const SystemType &type) const
		{
			return (Get(type) != nullptr);
		}
		
		template<class T>
		T *Get(const SystemType &type) const
		{
			return dynamic_cast<T*>(Get(type));
		}
		System *Get(const SystemType &type) const;

		void StartAll();
		void StopAll();

		void UpdateAll(float deltaTime);

	private:
		bool AddImpl(const SystemType &type, System *system);
		bool InsertAfterImpl(const SystemType &type, const SystemType &after, System *system);
		bool InsertBeforeImpl(const SystemType &type, const SystemType &before, System *system);

		class Impl;
		Impl *p;

		Application &application;
	};
}

#endif // PZ_SystemList_h__
