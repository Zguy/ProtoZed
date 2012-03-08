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

#include <cassert>

namespace PZ
{
	Property::Property(Type type) : type(type)
	{
	}
	Property::Property() : type(INVALID)
	{
	}
	Property::~Property()
	{
	}

	Property *Property::Create(Type type)
	{
		Property *property = nullptr;

		switch (type)
		{
		case STRING : property = new StringProperty; break;
		case INT    : property = new IntProperty;    break;
		case FLOAT  : property = new FloatProperty;  break;
		case BOOL   : property = new BoolProperty;   break;
		default     : property = new Property;       break;
		}

		return property;
	}
	void Property::Destroy(Property *&property)
	{
		delete property;
		property = nullptr;
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


	StringProperty::StringProperty() : Property(STRING), str("")
	{
	}
	StringProperty::StringProperty(const std::string &str) : Property(STRING), str(str)
	{
	}
	StringProperty::~StringProperty()
	{
	}

	const StringProperty &StringProperty::operator=(const std::string &str)
	{
		this->str = str;

		return *this;
	}


	IntProperty::IntProperty() : Property(INT), value(0)
	{
	}
	IntProperty::IntProperty(int value) : Property(INT), value(value)
	{
	}
	IntProperty::~IntProperty()
	{
	}

	const IntProperty &IntProperty::operator=(int value)
	{
		this->value = value;

		return *this;
	}


	FloatProperty::FloatProperty() : Property(FLOAT), value(0.f)
	{
	}
	FloatProperty::FloatProperty(float value) : Property(FLOAT), value(value)
	{
	}
	FloatProperty::~FloatProperty()
	{
	}

	const FloatProperty &FloatProperty::operator=(float value)
	{
		this->value = value;

		return *this;
	}


	BoolProperty::BoolProperty() : Property(BOOL), value(false)
	{
	}
	BoolProperty::BoolProperty(bool value) : Property(BOOL), value(value)
	{
	}
	BoolProperty::~BoolProperty()
	{
	}

	const BoolProperty &BoolProperty::operator=(bool value)
	{
		this->value = value;

		return *this;
	}
}