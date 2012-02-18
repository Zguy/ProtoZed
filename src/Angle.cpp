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
#include <ProtoZed/Angle.h>

#include <ProtoZed/Math.h>

namespace PZ
{
	namespace Angle
	{
		Degrees::Degrees() : angle(0.f)
		{
		}
		Degrees::Degrees(float angle) : angle(angle)
		{
		}
		Degrees::Degrees(const Radians &angle) : angle(angle.ToFloat() * (180.f / Math::PI))
		{
		}
		Degrees::~Degrees()
		{
		}

		Degrees &Degrees::operator=(float other)
		{
			angle = other;
			return *this;
		}
		Degrees &Degrees::operator=(const Radians &other)
		{
			angle = other.ToFloat() * (180.f / Math::PI);
			return *this;
		}

		Degrees &Degrees::operator+=(const Degrees &other)
		{
			angle += other.angle;
			return *this;
		}
		Degrees &Degrees::operator-=(const Degrees &other)
		{
			angle -= other.angle;
			return *this;
		}
		const Degrees Degrees::operator+(const Degrees &other) const
		{
			return Degrees(*this) += other;
		}
		const Degrees Degrees::operator-(const Degrees &other) const
		{
			return Degrees(*this) -= other;
		}

		Degrees &Degrees::operator-()
		{
			angle = -angle;
			return *this;
		}


		Radians::Radians() : angle(0.f)
		{
		}
		Radians::Radians(float angle) : angle(angle)
		{
		}
		Radians::Radians(const Degrees &angle) : angle(angle.ToFloat() * (Math::PI / 180.f))
		{
		}
		Radians::~Radians()
		{
		}

		Radians &Radians::operator=(float other)
		{
			angle = other;
			return *this;
		}
		Radians &Radians::operator=(const Degrees &other)
		{
			angle = other.ToFloat() * (Math::PI / 180.f);
			return *this;
		}

		Radians &Radians::operator+=(const Radians &other)
		{
			angle += other.angle;
			return *this;
		}
		Radians &Radians::operator-=(const Radians &other)
		{
			angle -= other.angle;
			return *this;
		}
		const Radians Radians::operator+(const Radians &other) const
		{
			return Radians(*this) += other;
		}
		const Radians Radians::operator-(const Radians &other) const
		{
			return Radians(*this) -= other;
		}

		Radians &Radians::operator-()
		{
			angle = -angle;
			return *this;
		}
	}
}