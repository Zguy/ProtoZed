/*
Copyright (c) 2013 Johannes H�ggqvist

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
#ifndef PZ_TypeInfo_h__
#define PZ_TypeInfo_h__

#include <typeinfo>

namespace PZ
{
	class TypeInfo
	{
	public:
		explicit TypeInfo(const std::type_info &info) : info(&info)
		{}
		TypeInfo(const TypeInfo &other) : info(other.info)
		{}
		~TypeInfo()
		{}

		TypeInfo &operator=(const TypeInfo &rhs)
		{
			if (&rhs != this)
			{
				info = rhs.info;
			}
			return *this;
		}

		bool operator<(const TypeInfo &rhs) const
		{
			return (info->before(*rhs.info) != 0);
		}
		
		bool operator==(const TypeInfo &rhs) const
		{
			return (*info == *rhs.info);
		}
		bool operator!=(const TypeInfo &rhs) const
		{
			return (*info != *rhs.info);
		}

	private:
		const std::type_info *info;
	};
}

#endif // PZ_TypeInfo_h__
