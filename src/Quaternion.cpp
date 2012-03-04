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
#include <ProtoZed/Quaternion.h>

namespace PZ
{
	Quaternion::Quaternion(float x, float y, float z , float w) : x(x), y(y), z(z), w(w)
	{
	}
	Quaternion::Quaternion(const Vector3f &axis, Angle::Radians angle)
	{
		float halfAngle = angle.ToFloat() * 0.5f;
		float sinAngle  = std::sin(halfAngle);

		x = axis.x * sinAngle;
		y = axis.y * sinAngle;
		z = axis.z * sinAngle;
		w = std::cos(halfAngle);
	}
	Quaternion::Quaternion(const Quaternion &other) : x(other.x), y(other.y), z(other.z), w(other.w)
	{
	}
	Quaternion::~Quaternion()
	{
	}

	Quaternion &Quaternion::operator=(const Quaternion &rhs)
	{
		if (this != &rhs)
		{
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
			w = rhs.w;
		}

		return *this;
	}

	Quaternion &Quaternion::operator+=(const Quaternion &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;

		return *this;
	}
	Quaternion &Quaternion::operator-=(const Quaternion &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;

		return *this;
	}
	Quaternion &Quaternion::operator*=(const Quaternion &rhs)
	{
		float tx = x, ty = y, tz = z, tw = w;

		x = tw * rhs.x + tx * rhs.w + ty * rhs.z - tz * rhs.y;
		y = tw * rhs.y + ty * rhs.w + tz * rhs.x - tx * rhs.z;
		z = tw * rhs.z + tz * rhs.w + tx * rhs.y - ty * rhs.x;
		w = tw * rhs.w - tx * rhs.x - ty * rhs.y - tz * rhs.z;

		return *this;
	}
	Quaternion &Quaternion::operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;

		return *this;
	}

	PZ::Vector3f Quaternion::operator*(const Vector3f &v) const
	{
		Vector3f uv, uuv;
		Vector3f qvec(x, y, z);
		uv = qvec.Cross(v);
		uuv = qvec.Cross(uv);
		uv *= (2.f * w);
		uuv *= 2.f;

		return v + uv + uuv;
	}

	bool Quaternion::operator==(const Quaternion &other) const
	{
		return (x == other.x)&&(y == other.y)&&(z == other.z)&&(w == other.w);
	}

	Quaternion Quaternion::Inverse() const
	{
		float norm = x*x+y*y+z*z+w*w;
		if (norm > 0.f)
		{
			float invNorm = 1.f / norm;
			return Quaternion(-x*invNorm, -y*invNorm, -z*invNorm, w*invNorm);
		}
		else
		{
			return Quaternion(0.f, 0.f, 0.f, 0.f); // Error
		}
	}
}
