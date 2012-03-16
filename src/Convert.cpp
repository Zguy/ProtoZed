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
#include <ProtoZed/Convert.h>

#include <boost/algorithm/string.hpp>

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
			return "("+Convert::ToString(value.x)+", "+Convert::ToString(value.y)+")";
		}
		template<> Vector2f FromString<Vector2f>(const std::string &str)
		{
			std::string left  = str.substr(0, str.find(','));
			std::string right = str.substr(str.find(',')+1);

			if (left.find('(') != std::string::npos)
				left = left.substr(left.find('(')+1);
			if (right.find(')') != std::string::npos)
				right = right.substr(0, right.find(')'));

			boost::trim(left);
			boost::trim(right);

			return Vector2f(Convert::FromString<float>(left), Convert::FromString<float>(right));
		}
	}
}