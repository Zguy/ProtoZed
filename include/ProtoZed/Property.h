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
#ifndef PZ_Property_h__
#define PZ_Property_h__

#include <string>

namespace PZ
{
	class PropertyList;

	class StringProperty;
	class IntProperty;
	class FloatProperty;
	class BoolProperty;

	class Property
	{
	public:
		enum Type
		{
			INVALID,
			STRING,
			INT,
			FLOAT,
			BOOL
		};

	protected:
		Property(const std::string &name, Type type, PropertyList *list);

	public:
		Property(const std::string &name, PropertyList *list);
		Property();
		virtual ~Property();

		const std::string &GetName() const
		{
			return name;
		}
		Type GetType() const
		{
			return type;
		}

		bool IsValid() const
		{
			return (!name.empty() && type != INVALID && list != nullptr);
		}

		static Property *Create(const std::string &name, Type type, PropertyList *list);
		static void Destroy(Property *&prop);

		const StringProperty &AsString() const;
		StringProperty &AsString();
		const IntProperty &AsInt() const;
		IntProperty &AsInt();
		const FloatProperty &AsFloat() const;
		FloatProperty &AsFloat();
		const BoolProperty &AsBool() const;
		BoolProperty &AsBool();

		const StringProperty &operator=(const std::string &str);
		const IntProperty &operator=(int value);
		const FloatProperty &operator=(float value);
		const BoolProperty &operator=(bool value);

	protected:
		void NotifyList();

	private:
		std::string name;
		Type type;

		PropertyList *list;
	};

	class StringProperty : public Property
	{
	public:
		StringProperty(const std::string &name, PropertyList *list);
		StringProperty(const std::string &name, const std::string &str, PropertyList *list);
		~StringProperty();

		const StringProperty &operator=(const std::string &str);

		operator const std::string&() const
		{
			return str;
		}

	private:
		std::string str;
	};

	class IntProperty : public Property
	{
	public:
		IntProperty(const std::string &name, PropertyList *list);
		IntProperty(const std::string &name, int value, PropertyList *list);
		~IntProperty();

		const IntProperty &operator=(int value);

		operator int() const
		{
			return value;
		}

	private:
		int value;
	};

	class FloatProperty : public Property
	{
	public:
		FloatProperty(const std::string &name, PropertyList *list);
		FloatProperty(const std::string &name, float value, PropertyList *list);
		~FloatProperty();

		const FloatProperty &operator=(float value);

		operator float() const
		{
			return value;
		}

	private:
		float value;
	};

	class BoolProperty : public Property
	{
	public:
		BoolProperty(const std::string &name, PropertyList *list);
		BoolProperty(const std::string &name, bool value, PropertyList *list);
		~BoolProperty();

		const BoolProperty &operator=(bool value);

		operator bool() const
		{
			return value;
		}

	private:
		bool value;
	};
}

#endif // PZ_Property_h__