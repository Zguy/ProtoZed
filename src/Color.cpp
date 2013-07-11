/*
Copyright (c) 2013 Johannes Häggqvist

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
#include <ProtoZed/Color.h>

namespace PZ
{
	const Color Color::Black(0.f, 0.f, 0.f);
	const Color Color::White(1.f, 1.f, 1.f);
	const Color Color::Red(1.f, 0.f, 0.f);
	const Color Color::Green(0.f, 1.f, 0.f);
	const Color Color::Blue(0.f, 0.f, 1.f);
	const Color Color::Yellow(1.f, 1.f, 0.f);
	const Color Color::Magenta(1.f, 0.f, 1.f);
	const Color Color::Cyan(0.f, 1.f, 1.f);
	const Color Color::Transparent(0.f, 0.f, 0.f, 0.f);

	Color::Color() : r(1.f), g(1.f), b(1.f), a(1.f)
	{
	}
	Color::Color(float r, float g, float b, float a)
	{
		SetColor(r, g, b, a);
	}
	Color::Color(int rgba)
	{
		SetFromRGBA(rgba);
	}
	Color::Color(const Color &other) : r(other.r), g(other.g), b(other.b), a(other.a)
	{
	}
	Color::~Color()
	{
	}

	void Color::SetColor(float r, float g, float b, float a)
	{
		SetRed(r);
		SetGreen(g);
		SetBlue(b);
		SetAlpha(a);
	}

	int Color::GetRGBA() const
	{
		return static_cast<int>(r * 255) << 24 | static_cast<int>(g * 255) << 16 | static_cast<int>(b * 255) << 8 | static_cast<int>(a * 255);
	}
	void Color::SetFromRGBA(int rgba)
	{
		r = static_cast<float>((rgba >> 24) & 0xFF) / 255.f;
		g = static_cast<float>((rgba >> 16) & 0xFF) / 255.f;
		b = static_cast<float>((rgba >> 8) & 0xFF) / 255.f;
		a = static_cast<float>((rgba) & 0xFF) / 255.f;
	}

	Color &Color::operator+=(const Color &rhs)
	{
		SetRed(r + rhs.r);
		SetGreen(g + rhs.g);
		SetBlue(b + rhs.b);
		SetAlpha(a + rhs.a);
		return *this;
	}
	Color &Color::operator-=(const Color &rhs)
	{
		SetRed(r - rhs.r);
		SetGreen(g - rhs.g);
		SetBlue(b - rhs.b);
		SetAlpha(a - rhs.a);
		return *this;
	}
	Color &Color::operator*=(const Color &rhs)
	{
		SetRed(r * rhs.r);
		SetGreen(g * rhs.g);
		SetBlue(b * rhs.b);
		SetAlpha(a * rhs.a);
		return *this;
	}
	Color &Color::operator/=(const Color &rhs)
	{
		SetRed(r / rhs.r);
		SetGreen(g / rhs.g);
		SetBlue(b / rhs.b);
		SetAlpha(a / rhs.a);
		return *this;
	}

	const Color Color::operator+(const Color &rhs) const
	{
		return Color(*this) += rhs;
	}
	const Color Color::operator-(const Color &rhs) const
	{
		return Color(*this) -= rhs;
	}
	const Color Color::operator*(const Color &rhs) const
	{
		return Color(*this) *= rhs;
	}
	const Color Color::operator/(const Color &rhs) const
	{
		return Color(*this) /= rhs;
	}

	Color &Color::operator=(const Color &rhs)
	{
		if (this != &rhs)
		{
			r = rhs.r;
			g = rhs.g;
			b = rhs.b;
			a = rhs.a;
		}
		return *this;
	}
}