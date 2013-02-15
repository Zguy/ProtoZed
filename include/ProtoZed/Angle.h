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
#ifndef PZ_Angle_h__
#define PZ_Angle_h__

namespace PZ
{
	namespace Angle
	{
		class Degrees;
		class Radians;

		class Degrees
		{
		public:
			Degrees();
			explicit Degrees(float angle);
			Degrees(const Radians &angle);
			~Degrees();

			Degrees &operator=(float other);
			Degrees &operator=(const Radians &other);

			Degrees &operator+=(const Degrees &other);
			Degrees &operator-=(const Degrees &other);
			const Degrees operator+(const Degrees &other) const;
			const Degrees operator-(const Degrees &other) const;

			Degrees &operator-();

			inline float ToFloat() const
			{
				return angle;
			}

		private:
			float angle;
		};

		class Radians
		{
		public:
			Radians();
			explicit Radians(float angle);
			Radians(const Degrees &angle);
			~Radians();

			Radians &operator=(float other);
			Radians &operator=(const Degrees &other);

			Radians &operator+=(const Radians &other);
			Radians &operator-=(const Radians &other);
			const Radians operator+(const Radians &other) const;
			const Radians operator-(const Radians &other) const;

			Radians &operator-();

			inline float ToFloat() const
			{
				return angle;
			}

		private:
			float angle;
		};
	}
}

#endif // PZ_Angle_h__
