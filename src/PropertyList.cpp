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
#include <ProtoZed/PropertyList.h>

namespace PZ
{
	Property PropertyList::invalidProperty = Property();

	PropertyList::PropertyList()
	{
	}
	PropertyList::~PropertyList()
	{
		Clear();
	}

	bool PropertyList::Add(const std::string &name, Property::Type type)
	{
		return properties.insert(std::make_pair(name, Property::Create(type))).second;
	}
	bool PropertyList::Remove(const std::string &name)
	{
		PropertyMap::iterator it = properties.find(name);
		if (it != properties.end())
		{
			Property::Destroy((*it).second);
			properties.erase(it);

			return true;
		}

		return false;
	}
	void PropertyList::Clear()
	{
		for (PropertyMap::iterator it = properties.begin(); it != properties.end(); ++it)
		{
			Property::Destroy((*it).second);
		}
		properties.clear();
	}

	Property &PropertyList::Get(const std::string &name)
	{
		PropertyMap::iterator it = properties.find(name);
		if (it != properties.end())
		{
			return *(*it).second;
		}

		return invalidProperty;
	}
	const Property &PropertyList::Get(const std::string &name) const
	{
		PropertyMap::const_iterator it = properties.find(name);
		if (it != properties.end())
		{
			return *(*it).second;
		}

		return invalidProperty;
	}
}