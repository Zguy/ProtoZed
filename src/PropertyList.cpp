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
#include <ProtoZed/PropertyList.h>

namespace PZ
{
	PropertyList::PropertyList()
	{
	}
	PropertyList::~PropertyList()
	{
		ClearProperties();
	}

	bool PropertyList::AddProperty(PropertyBase &prop)
	{
		if (_AddProperty(&prop))
		{
			prop.list = this;
			return true;
		}
		
		return false;
	}
	bool PropertyList::RemoveProperty(const std::string &name)
	{
		PropertyMap::iterator it = properties.find(name);
		if (it != properties.end())
		{
			PropertyBase *prop = (*it).second;
			prop->list = nullptr;

			properties.erase(it);

			return true;
		}

		return false;
	}
	void PropertyList::ClearProperties()
	{
		for (PropertyMap::iterator it = properties.begin(); it != properties.end(); ++it)
		{
			PropertyBase *&prop = (*it).second;
			prop->list = nullptr;

			prop = nullptr;
		}
		properties.clear();
	}

	bool PropertyList::HasProperty(const std::string &name) const
	{
		return (properties.find(name) != properties.end());
	}

	PropertyBase &PropertyList::GetProperty(const std::string &name)
	{
		PropertyBase *prop = nullptr;

		PropertyMap::iterator it = properties.find(name);
		if (it != properties.end())
		{
			prop = (*it).second;
		}

		assert(prop != nullptr);
		return *prop;
	}
	const PropertyBase &PropertyList::GetProperty(const std::string &name) const
	{
		const PropertyBase *prop = nullptr;

		PropertyMap::const_iterator it = properties.find(name);
		if (it != properties.end())
		{
			prop = (*it).second;
		}

		assert(prop != nullptr);
		return *prop;
	}

	bool PropertyList::_AddProperty(PropertyBase *prop)
	{
		if (prop != nullptr && prop->IsValid() && !prop->IsInList())
			return properties.insert(std::make_pair(prop->GetName(), prop)).second;
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