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
#ifndef Singleton_h__
#define Singleton_h__

#include <cassert>

namespace PZ
{
	template<typename T>
	class Singleton
	{
	public:
		Singleton()
		{
			assert(!ms_singleton);
			int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
			ms_singleton = (T*)((int)this + offset);
		}
		~Singleton()
		{
			assert(ms_singleton);
			ms_singleton=0;
		}
		static T &GetSingleton()
		{
			assert(ms_singleton);
			return *ms_singleton;
		}
		static T *GetSingletonPtr()
		{
			assert(ms_singleton);
			return ms_singleton;
		}

	private:
		static T *ms_singleton;
	};
	
	template<typename T> T *Singleton <T>::ms_singleton = 0;
}

#endif // Singleton_h__