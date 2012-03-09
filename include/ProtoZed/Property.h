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
		Property(Type type);

	public:
		Property();
		virtual ~Property();

		Type GetType() const
		{
			return type;
		}

		bool IsValid() const
		{
			return type != INVALID;
		}

		static Property *Create(Type type);
		static void Destroy(Property *&property);

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

	private:
		Type type;
	};

	class StringProperty : public Property
	{
	public:
		StringProperty();
		StringProperty(const std::string &str);
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
		IntProperty();
		IntProperty(int value);
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
		FloatProperty();
		FloatProperty(float value);
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
		BoolProperty();
		BoolProperty(bool value);
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