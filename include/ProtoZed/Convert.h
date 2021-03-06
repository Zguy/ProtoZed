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
#ifndef PZ_Convert_h__
#define PZ_Convert_h__

#include <ProtoZed/Vector2.h>
#include <ProtoZed/Angle.h>
#include <ProtoZed/Color.h>

#include <boost/lexical_cast.hpp>

#include <string>

namespace PZ
{
	namespace Convert
	{
		template<typename T>
		std::string ToString(const T &value)
		{
			return boost::lexical_cast<std::string, T>(value);
		}
		template<typename T>
		T FromString(const std::string &str)
		{
			return boost::lexical_cast<T, std::string>(str);
		}

		// Bool
		template<> std::string ToString<bool>(const bool &value);
		template<> bool FromString<bool>(const std::string &str);

		// Vector2
		template<> std::string ToString<Vector2f>(const Vector2f &value);
		template<> Vector2f FromString<Vector2f>(const std::string &str);

		// Angle
		template<> std::string ToString<Angle::Degrees>(const Angle::Degrees &value);
		template<> Angle::Degrees FromString<Angle::Degrees>(const std::string &str);
		template<> std::string ToString<Angle::Radians>(const Angle::Radians &value);
		template<> Angle::Radians FromString<Angle::Radians>(const std::string &str);

		// Color
		template<> std::string ToString<Color>(const Color &value);
		template<> Color FromString<Color>(const std::string &str);
	}
}

#endif // PZ_Convert_h__