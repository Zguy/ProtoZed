/*
Copyright (c) 2013 Johannes Häggqvist

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
#ifndef PZ_Property_h__
#define PZ_Property_h__

#include <ProtoZed/Convert.h>

#include <boost/noncopyable.hpp>

#include <string>

namespace PZ
{
	class PropertyList;

	class PropertyBase : public boost::noncopyable
	{
		friend class PropertyList;

	public:
		explicit PropertyBase(const std::string &name, PropertyList *list = nullptr);
		virtual ~PropertyBase();

		const std::string &GetName() const
		{
			return name;
		}

		bool IsValid() const
		{
			return (!name.empty());
		}
		bool IsInList() const
		{
			return (list != nullptr);
		}

		void LockNotify()
		{
			++locked;
		}
		void UnlockNotify()
		{
			--locked;
		}
		bool IsNotifyLocked()
		{
			return (locked != 0);
		}

		// Is this the best way?
		virtual std::string ToString() const = 0;
		virtual void FromString(const std::string &str) = 0;

		bool operator==(const PropertyBase &other) const
		{
			return (name == other.name && list == other.list);
		}
		bool operator!=(const PropertyBase &other) const
		{
			return !((*this) == other);
		}

	protected:
		void NotifyList();

	private:
		std::string name;

		PropertyList *list;

		int locked;
	};

	template<typename T>
	class Property : public PropertyBase
	{
	public:
		explicit Property(const std::string &name, PropertyList *list = nullptr) : PropertyBase(name, list)
		{
		}
		Property(const std::string &name, const T &value, PropertyList *list = nullptr) : PropertyBase(name, list), value(value)
		{
		}
		~Property()
		{
		}

		const Property &operator=(const T &value)
		{
			this->value = value;

			NotifyList();

			return *this;
		}

		const T &Get() const
		{
			return value;
		}

		operator const T&() const
		{
			return value;
		}

		virtual std::string ToString() const
		{
			return Convert::ToString(value);
		}
		virtual void FromString(const std::string &str)
		{
			(*this) = Convert::FromString<T>(str);
		}

	private:
		T value;
	};
}

#endif // PZ_Property_h__
