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
#ifndef PZ_Quaternion_h__
#define PZ_Quaternion_h__

#include <ProtoZed/Vector3.h>
#include <ProtoZed/Angle.h>

namespace PZ
{
	class Quaternion
	{
	public:
		Quaternion(float x = 0.f, float y = 0.f, float z = 0.f, float w = 1.f);
		Quaternion(const Vector3f &axis, Angle::Radians angle);
		Quaternion(const Quaternion &other);
		~Quaternion();

		Quaternion &operator=(const Quaternion &rhs);

		Quaternion &operator+=(const Quaternion &rhs);
		Quaternion &operator-=(const Quaternion &rhs);
		Quaternion &operator*=(const Quaternion &rhs);
		Quaternion &operator*=(float scalar);

		const Quaternion operator+(const Quaternion &other) const
		{
			return Quaternion(*this) += other;
		}
		const Quaternion operator-(const Quaternion &other) const
		{
			return Quaternion(*this) -= other;
		}
		const Quaternion operator*(const Quaternion &other) const
		{
			return Quaternion(*this) *= other;
		}
		const Quaternion operator*(float scalar) const
		{
			return Quaternion(*this) *= scalar;
		}

		bool operator==(const Quaternion &other) const;
		bool operator!=(const Quaternion &other) const
		{
			return !(*this == other);
		}

		float x, y, z, w;
	};
}

#endif // PZ_Quaternion_h__
