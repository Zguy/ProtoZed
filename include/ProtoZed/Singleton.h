/*
Copyright (c) 2012 Johannes H�ggqvist

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
// Reference: http://scottbilas.com/publications/gem-singleton/
#ifndef PZ_Singleton_h__
#define PZ_Singleton_h__

#include <cassert>

namespace PZ
{
	template<typename T>
	class Singleton
	{
	public:
		Singleton()
		{
			assert(!singleton);
			singleton = static_cast<T*>(this);
		}
		~Singleton()
		{
			assert(singleton);
			singleton = nullptr;
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
	
	template<typename T> T *Singleton<T>::singleton = nullptr;
}

#endif // PZ_Singleton_h__