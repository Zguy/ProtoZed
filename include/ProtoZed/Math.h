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
#ifndef PZ_Math_h__
#define PZ_Math_h__

#include <cassert>

namespace PZ
{
	namespace Math
	{
		static const float PI      = 3.14159265358979323846f;
		static const float PI_2    = 6.28318530717958647693f;
		static const float PI_HALF = 1.57079632679489661923f;

		float Sqrt(float value);
		float Sin(float value);
		float Cos(float value);
		float Tan(float value);
		float ASin(float value);
		float ACos(float value);
		float ATan(float value);
		float ATan2(float y, float x);
		float Pow(float value, float pow);
		float Floor(float value);
		float Ceil(float value);

		float Abs(float value);
		double Abs(double value);
		int Abs(int value);

		template<typename T>
		T Clamp(T value, T min, T max)
		{
			assert(min <= max);

			if (value < min)
				return min;
			else if (value > max)
				return max;
			else
				return value;
		}
		template<typename T>
		T Wrap(T value, T min, T max)
		{
			assert(min <= max);

			T range_size = max - min + 1;

			if (value < min)
				value += range_size * ((min - value) / range_size + 1);

			return min + (value - min) % range_size;
		}

		template<typename T>
		T Min(T a, T b)
		{
			return (a < b ? a : b);
		}
		template<typename T>
		T Max(T a, T b)
		{
			return (a < b ? b : a);
		}
	}
}

#endif // PZ_Math_h__
