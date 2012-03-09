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
#include <ProtoZed/Timestamp.h>

#include <boost/chrono.hpp>

namespace PZ
{
	class Timestamp::Impl
	{
	public:
		boost::chrono::high_resolution_clock::time_point stamp;
	};

	Timestamp::Timestamp() : p(new Impl)
	{
		Now();
	}
	Timestamp::Timestamp(const Timestamp &other) : p(new Impl)
	{
		p->stamp = other.p->stamp;
	}
	Timestamp::~Timestamp()
	{
		delete p;
	}

	const Timestamp &Timestamp::operator=(const Timestamp &rhs)
	{
		if (this != &rhs)
		{
			p->stamp = rhs.p->stamp;
		}

		return *this;
	}

	bool Timestamp::operator==(const Timestamp &other) const
	{
		return p->stamp == other.p->stamp;
	}

	void Timestamp::Now()
	{
		p->stamp = boost::chrono::high_resolution_clock::now();
	}
}
