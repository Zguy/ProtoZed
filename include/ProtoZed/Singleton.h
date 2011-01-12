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
// Reference: http://scottbilas.com/publications/gem-singleton/
#ifndef Singleton_h__
#define Singleton_h__

#include <cassert>
#include <cstdlib>

namespace PZ
{
	template<typename T>
	class Singleton
	{
	public:
		Singleton()
		{
			assert(!singleton);
#if defined(_MSC_VER) && _MSC_VER < 1200
			int offset = (int)(T*)1 - (int)(Singleton<T>*)(T*)1;
			singleton = (T*)((int)this + offset);
#else
			singleton = static_cast<T*>(this);
#endif
		}
		~Singleton()
		{
			assert(singleton);
			singleton = NULL;
		}
		static T &GetSingleton()
		{
			assert(singleton);
			return *singleton;
		}
		static T *GetSingletonPtr()
		{
			return singleton;
		}

	private:
		static T *singleton;
	};
	
	template<typename T> T *Singleton<T>::singleton = 0;
}

#endif // Singleton_h__