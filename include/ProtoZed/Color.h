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
#ifndef PZ_Color_h__
#define PZ_Color_h__

#include <ProtoZed/Math.h>

namespace PZ
{
	/**
	 * \brief	RGBA color with normalized components (0-1).
	 */
	class Color
	{
	public:
		// Predefined colors
		static const Color Black;
		static const Color White;
		static const Color Red;
		static const Color Green;
		static const Color Blue;
		static const Color Yellow;
		static const Color Magenta;
		static const Color Cyan;
		static const Color Transparent;

		Color();
		Color(float r, float g, float b, float a = 1.f);
		explicit Color(int rgba);
		Color(const Color &other);
		~Color();

		inline float GetRed() const { return r; }
		inline float GetGreen() const { return g; }
		inline float GetBlue() const { return b; }
		inline float GetAlpha() const { return a; }

		void SetColor(float r, float g, float b, float a = 1.f);
		inline void SetRed(float red)
		{
			r = Math::Clamp(red, 0.f, 1.f);
		}
		inline void SetGreen(float green)
		{
			g = Math::Clamp(green, 0.f, 1.f);
		}
		inline void SetBlue(float blue)
		{
			b = Math::Clamp(blue, 0.f, 1.f);
		}
		inline void SetAlpha(float alpha)
		{
			a = Math::Clamp(alpha, 0.f, 1.f);
		}

		int GetRGBA() const;
		void SetFromRGBA(int rgba);

		Color &operator+=(const Color &rhs);
		Color &operator-=(const Color &rhs);
		Color &operator*=(const Color &rhs);
		Color &operator/=(const Color &rhs);

		const Color operator+(const Color &rhs) const;
		const Color operator-(const Color &rhs) const;
		const Color operator*(const Color &rhs) const;
		const Color operator/(const Color &rhs) const;

		Color &operator=(const Color &rhs);

	private:
		float r;
		float g;
		float b;
		float a;
	};
}

#endif // PZ_Color_h__