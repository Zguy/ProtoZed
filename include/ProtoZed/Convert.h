/*
	Copyright 2010-2011 Johannes Häggqvist

	This file is part of ProtoZed.

	ProtoZed is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	ProtoZed is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with ProtoZed.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef Convert_h__
#define Convert_h__

#include <string>
#include <sstream>

namespace PZ
{
	static const float PI = 3.14159265358979323846f;

	namespace Convert
	{
		template<typename T>
		std::string ToString(const T &value)
		{
			std::stringstream sstr;
			sstr << value;
			return sstr.str();
		}
		template<typename T>
		T FromString(const std::string &str)
		{
			std::stringstream sstr(str);
			T value;
			sstr >> value;
			return value;
		}

		float DegreesToRadians(float degrees);
		float RadiansToDegrees(float radians);
	}
}

#endif // Convert_h__