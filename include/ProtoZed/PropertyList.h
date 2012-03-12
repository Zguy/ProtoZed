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
#ifndef PZ_PropertyList_h__
#define PZ_PropertyList_h__

#include <ProtoZed/Property.h>

#include <string>
#include <map>

namespace PZ
{
	typedef std::map<std::string, Property*> PropertyMap;

	class PropertyList
	{
		friend class Property;

	public:
		PropertyList();
		virtual ~PropertyList();

		bool AddProperty(const std::string &name, Property::Type type);
		bool RemoveProperty(const std::string &name);
		void ClearProperties();

		bool HasProperty(const std::string &name) const;

		Property &GetProperty(const std::string &name);
		const Property &GetProperty(const std::string &name) const;

		const PropertyMap &GetAllProperties() const
		{
			return properties;
		}

	protected:
		// We pass a const Property because if it is changed within this method we might get infinite recursion
		virtual void PropertyUpdated(const std::string &name, const Property &prop)
		{}

	private:
		bool _AddProperty(const std::string &name, Property *prop);
		bool _RemoveProperty(const std::string &name);

		PropertyMap properties;

		static Property invalidProperty;
	};
}

#endif // PZ_PropertyList_h__