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
#include <ProtoZed/HashString.h>

#include <ProtoZed/Convert.h>
#include <ProtoZed/xxhash.h>

static const unsigned int XXHASH_SEED = 0;

namespace PZ
{
	HashString::HashString() : hash(0)
	{
#ifdef _DEBUG
		readable = "";
#endif
	}
	HashString::HashString(const HashString &other) : hash(other.hash)
	{
#ifdef _DEBUG
		readable = other.readable;
#endif
	}
	HashString::HashString(const std::string &str)
	{
		hash = hashString(str);
#ifdef _DEBUG
		readable = str;
#endif
	}
	HashString::HashString(const char *str)
	{
		hash = hashString(std::string(str));
#ifdef _DEBUG
		readable = str;
#endif
	}
	HashString::HashString(unsigned int hash) : hash(hash)
	{
#ifdef _DEBUG
		readable = Convert::ToString(hash);
#endif
	}
	HashString::~HashString()
	{
	}

	HashString &HashString::operator=(const HashString &other)
	{
		if (&other != this)
		{
			hash = other.hash;
#ifdef _DEBUG
			readable = other.readable;
#endif
		}

		return *this;
	}
	HashString &HashString::operator=(const std::string &str)
	{
		hash = hashString(str);
#ifdef _DEBUG
		readable = str;
#endif
		return *this;
	}

	bool HashString::operator==(const HashString &other) const
	{
		return (hash == other.hash);
	}
	bool HashString::operator!=(const HashString &other) const
	{
		return (hash != other.hash);
	}
	bool HashString::operator<(const HashString &other) const
	{
		return (hash < other.hash);
	}

#ifndef _DEBUG
	const std::string HashString::GetReadable() const
	{
		return Convert::ToString(hash);
	}
#endif

	unsigned int HashString::hashString(const std::string &str) const
	{
		return XXH32(str.c_str(), str.size(), XXHASH_SEED);
	}
}