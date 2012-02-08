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
#include <ProtoZed/Helpers.h>

#include <ProtoZed/Math.h>

#include <cmath>

namespace PZ
{
	namespace Helpers
	{
		Vector2f GetForwardVector(Angle::Radians rotation, float length)
		{
			return Vector2f(std::cos(rotation.ToFloat()) * length * -1, std::sin(rotation.ToFloat()) * length);
		}

		Angle::Radians GetRotationTo(const Vector2f &from, const Vector2f &to)
		{
			float x = to.x - from.x;
			float y = to.y - from.y;

			return (Angle::Radians(std::atan2(x, y) + Math::PI));
		}
	}
}