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
	typedef std::map<std::string, PropertyBase*> PropertyMap;

	class PropertyList
	{
		friend class PropertyBase;

	public:
		PropertyList();
		virtual ~PropertyList();

		bool AddProperty(PropertyBase &prop);
		bool RemoveProperty(const std::string &name);
		void ClearProperties();

		bool HasProperty(const std::string &name) const;

		PropertyBase &GetProperty(const std::string &name);
		const PropertyBase &GetProperty(const std::string &name) const;

		template<typename T>
		void SetProperty(const std::string &name, const T &value)
		{
			GetProperty<T>(name) = value;
		}

		template<typename T>
		Property<T> &GetProperty(const std::string &name)
		{
			return dynamic_cast<Property<T>&>(GetProperty(name));
		}
		template<typename T>
		const Property<T> &GetProperty(const std::string &name) const
		{
			return dynamic_cast<const Property<T>&>(GetProperty(name));
		}

		const PropertyMap &GetAllProperties() const
		{
			return properties;
		}

	protected:
		virtual void PropertyUpdated(const PropertyBase &prop)
		{}

	private:
		PropertyMap properties;
	};
}

#endif // PZ_PropertyList_h__
