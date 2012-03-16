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
#include <ProtoZed/Property.h>

#include <ProtoZed/PropertyList.h>

#include <cassert>

namespace PZ
{
	Property::Property(const std::string &name, Type type, PropertyList *list) : name(name), type(type), list(list)
	{
		if (list != nullptr)
		{
			if (!list->_AddProperty(name, this))
			{
				list = nullptr;
			}
		}
	}
	Property::Property(const std::string &name, PropertyList *list) : name(name), type(INVALID), list(list)
	{
		if (list != nullptr)
		{
			if (!list->_AddProperty(name, this))
			{
				list = nullptr;
			}
		}
	}
	Property::Property() : name(""), type(INVALID), list(nullptr)
	{
	}
	Property::~Property()
	{
		if (list != nullptr)
		{
			list->_RemoveProperty(name);
		}
	}

	Property *Property::Create(const std::string &name, Type type, PropertyList *list)
	{
		Property *prop = nullptr;

		switch (type)
		{
		case STRING  : prop = new StringProperty(name, list);  break;
		case INT     : prop = new IntProperty(name, list);     break;
		case FLOAT   : prop = new FloatProperty(name, list);   break;
		case BOOL    : prop = new BoolProperty(name, list);    break;
		case VECTOR2 : prop = new Vector2Property(name, list); break;
		default      : prop = new Property(name, list);        break;
		}

		return prop;
	}
	void Property::Destroy(Property *&prop)
	{
		Property *toDelete = prop;
		prop = nullptr;
		delete toDelete;
	}

	const StringProperty &Property::AsString() const
	{
		assert(type == STRING);
		return *static_cast<const StringProperty*>(this);
	}
	StringProperty &Property::AsString()
	{
		assert(type == STRING);
		return *static_cast<StringProperty*>(this);
	}
	const IntProperty &Property::AsInt() const
	{
		assert(type == INT);
		return *static_cast<const IntProperty*>(this);
	}
	IntProperty &Property::AsInt()
	{
		assert(type == INT);
		return *static_cast<IntProperty*>(this);
	}
	const FloatProperty &Property::AsFloat() const
	{
		assert(type == FLOAT);
		return *static_cast<const FloatProperty*>(this);
	}
	FloatProperty &Property::AsFloat()
	{
		assert(type == FLOAT);
		return *static_cast<FloatProperty*>(this);
	}
	const BoolProperty &Property::AsBool() const
	{
		assert(type == BOOL);
		return *static_cast<const BoolProperty*>(this);
	}
	BoolProperty &Property::AsBool()
	{
		assert(type == BOOL);
		return *static_cast<BoolProperty*>(this);
	}
	const Vector2Property &Property::AsVector2() const
	{
		assert(type == VECTOR2);
		return *static_cast<const Vector2Property*>(this);
	}
	Vector2Property &Property::AsVector2()
	{
		assert(type == VECTOR2);
		return *static_cast<Vector2Property*>(this);
	}

	const StringProperty &Property::operator=(const std::string &str)
	{
		return (AsString() = str);
	}
	const IntProperty &Property::operator=(int value)
	{
		return (AsInt() = value);
	}
	const FloatProperty &Property::operator=(float value)
	{
		return (AsFloat() = value);
	}
	const BoolProperty &Property::operator=(bool value)
	{
		return (AsBool() = value);
	}
	const Vector2Property &Property::operator=(const Vector2f &value)
	{
		return (AsVector2() = value);
	}

	void Property::NotifyList()
	{
		list->PropertyUpdated(name, *this);
	}


	StringProperty::StringProperty(const std::string &name, PropertyList *list) : Property(name, STRING, list), str("")
	{
	}
	StringProperty::StringProperty(const std::string &name, const std::string &str, PropertyList *list) : Property(name, STRING, list), str(str)
	{
	}
	StringProperty::~StringProperty()
	{
	}

	const StringProperty &StringProperty::operator=(const std::string &str)
	{
		this->str = str;

		NotifyList();

		return *this;
	}


	IntProperty::IntProperty(const std::string &name, PropertyList *list) : Property(name, INT, list), value(0)
	{
	}
	IntProperty::IntProperty(const std::string &name, int value, PropertyList *list) : Property(name, INT, list), value(value)
	{
	}
	IntProperty::~IntProperty()
	{
	}

	const IntProperty &IntProperty::operator=(int value)
	{
		this->value = value;

		NotifyList();

		return *this;
	}


	FloatProperty::FloatProperty(const std::string &name, PropertyList *list) : Property(name, FLOAT, list), value(0.f)
	{
	}
	FloatProperty::FloatProperty(const std::string &name, float value, PropertyList *list) : Property(name, FLOAT, list), value(value)
	{
	}
	FloatProperty::~FloatProperty()
	{
	}

	const FloatProperty &FloatProperty::operator=(float value)
	{
		this->value = value;

		NotifyList();

		return *this;
	}


	BoolProperty::BoolProperty(const std::string &name, PropertyList *list) : Property(name, BOOL, list), value(false)
	{
	}
	BoolProperty::BoolProperty(const std::string &name, bool value, PropertyList *list) : Property(name, BOOL, list), value(value)
	{
	}
	BoolProperty::~BoolProperty()
	{
	}

	const BoolProperty &BoolProperty::operator=(bool value)
	{
		this->value = value;

		NotifyList();

		return *this;
	}


	Vector2Property::Vector2Property(const std::string &name, PropertyList *list) : Property(name, VECTOR2, list)
	{
	}
	Vector2Property::Vector2Property(const std::string &name, const Vector2f &value, PropertyList *list) : Property(name, VECTOR2, list), value(value)
	{
	}
	Vector2Property::~Vector2Property()
	{
	}

	const Vector2Property &Vector2Property::operator=(const Vector2f &value)
	{
		this->value = value;

		NotifyList();

		return *this;
	}
}