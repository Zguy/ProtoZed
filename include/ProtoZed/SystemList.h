/*
Copyright (c) 2013 Johannes Häggqvist

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

#include <ProtoZed/System.h>

#include <boost/noncopyable.hpp>

namespace PZ
{
	/**
	 * \brief	List of systems.
	 */
	class SystemList : boost::noncopyable
	{
	public:
		SystemList(Application &application);
		~SystemList();

		/**
		 * \brief	Adds a system at the end.
		 *
		 * \param	type	The type of the system.
		 *
		 * \return	null if it fails, else the system.
		 */
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

		/**
		 * \brief	Inserts a system after another system.
		 *
		 * \param	type 	The type of the system.
		 * \param	after	The type of the system to insert after.
		 *
		 * \return	null if it fails, else the system.
		 */
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

		/**
		 * \brief	Inserts a system before another system.
		 *
		 * \param	type		The type of the system.
		 * \param	before	The type of the system to insert before.
		 *
		 * \return	null if it fails, else the system.
		 */
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

		/**
		 * \brief	Removes a system.
		 *
		 * \param	type	The type of the system to remove.
		 *
		 * \return	true if it succeeds, false if it fails.
		 */
		bool Remove(const SystemType &type);

		/**
		 * \brief	Removes all systems.
		 */
		void RemoveAll();

		/**
		 * \brief	Query if a system is in the list.
		 *
		 * \param	type	The type of the system.
		 *
		 * \return	true if the system is in the list, false if not.
		 */
		inline bool Has(const SystemType &type) const
		{
			return (Get(type) != nullptr);
		}
		
		template<class T>
		T *Get(const SystemType &type) const
		{
			return dynamic_cast<T*>(Get(type));
		}

		/**
		 * \brief	Gets a system.
		 *
		 * \param	type	The type of the system.
		 *
		 * \return	null if it fails, else the system.
		 */
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
