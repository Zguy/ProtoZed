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
		ClearProperties();
	}

	bool PropertyList::AddProperty(const std::string &name, Property::Type type)
	{
		if (!name.empty())
		{
			Property::Create(name, type, this);
			return HasProperty(name);
		}

		return false;
	}
	bool PropertyList::RemoveProperty(const std::string &name)
	{
		PropertyMap::iterator it = properties.find(name);
		if (it != properties.end())
		{
			Property::Destroy((*it).second);
			return !HasProperty(name);
		}

		return false;
	}
	void PropertyList::ClearProperties()
	{
		while (!properties.empty())
		{
			Property::Destroy((*properties.begin()).second);
		}
	}

	bool PropertyList::HasProperty(const std::string &name) const
	{
		return (properties.find(name) != properties.end());
	}

	Property &PropertyList::GetProperty(const std::string &name)
	{
		PropertyMap::iterator it = properties.find(name);
		if (it != properties.end())
		{
			return *(*it).second;
		}

		return invalidProperty;
	}
	const Property &PropertyList::GetProperty(const std::string &name) const
	{
		PropertyMap::const_iterator it = properties.find(name);
		if (it != properties.end())
		{
			return *(*it).second;
		}

		return invalidProperty;
	}

	bool PropertyList::_AddProperty(const std::string &name, Property *prop)
	{
		if (prop != nullptr)
			return properties.insert(std::make_pair(name, prop)).second;
		else
			return false;
	}
	bool PropertyList::_RemoveProperty(const std::string &name)
	{
		PropertyMap::iterator it = properties.find(name);
		if (it != properties.end())
		{
			properties.erase(it);

			return true;
		}

		return false;
	}
}