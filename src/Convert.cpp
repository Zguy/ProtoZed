/*
Copyright (c) 2013 Johannes H�ggqvist

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
#include <ProtoZed/Convert.h>

#include <boost/algorithm/string.hpp>

#include <vector>

namespace PZ
{
	namespace Convert
	{
		// Bool
		template<> std::string ToString<bool>(const bool &value)
		{
			return (value ? "true" : "false");
		}
		template<> bool FromString<bool>(const std::string &str)
		{
			return (str == "true");
		}

		// Vector2
		template<> std::string ToString<Vector2f>(const Vector2f &value)
		{
			return ToString(value.x)+", "+ToString(value.y);
		}
		template<> Vector2f FromString<Vector2f>(const std::string &str)
		{
			std::string::size_type split = str.find(',');
			std::string left  = str.substr(0, split);
			std::string right = str.substr(split+1);

			boost::trim(left);
			boost::trim(right);

			return Vector2f(FromString<float>(left), FromString<float>(right));
		}

		// Angle
		template<> std::string ToString<Angle::Degrees>(const Angle::Degrees &value)
		{
			return ToString(value.ToFloat());
		}
		template<> Angle::Degrees FromString<Angle::Degrees>(const std::string &str)
		{
			return Angle::Degrees(FromString<float>(str));
		}
		template<> std::string ToString<Angle::Radians>(const Angle::Radians &value)
		{
			return ToString(value.ToFloat());
		}
		template<> Angle::Radians FromString<Angle::Radians>(const std::string &str)
		{
			return Angle::Radians(FromString<float>(str));
		}

		// Color
		namespace
		{
			bool is_comma(const char &c)
			{
				return (c == ',');
			}
			void trim(std::string &s)
			{
				boost::trim(s);
			}
		}
		template<> std::string ToString(const Color &value)
		{
			return ToString(value.GetRed())+", "+ToString(value.GetGreen())+", "+ToString(value.GetBlue())+", "+ToString(value.GetAlpha());
		}
		template<> Color FromString(const std::string &str)
		{
			std::vector<std::string> parts;
			boost::split(parts, str, is_comma);
			std::for_each(parts.begin(), parts.end(), trim);
			Color color;
			if (parts.size() > 0)
				color.SetRed(FromString<float>(parts[0]));
			if (parts.size() > 1)
				color.SetGreen(FromString<float>(parts[1]));
			if (parts.size() > 2)
				color.SetBlue(FromString<float>(parts[2]));
			if (parts.size() > 3)
				color.SetAlpha(FromString<float>(parts[3]));
			return color;
		}
	}
}