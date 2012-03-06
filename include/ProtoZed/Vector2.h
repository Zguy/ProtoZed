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
#ifndef PZ_Vector2_h__
#define PZ_Vector2_h__

#include <ProtoZed/Math.h>

namespace PZ
{
	template<typename T>
	class Vector2
	{
	public:
		Vector2() : x(0), y(0)
		{}
		Vector2(const Vector2<T> &other) : x(other.x), y(other.y)
		{}
		Vector2(T x, T y) : x(x), y(y)
		{}
		~Vector2()
		{}

		Vector2<T> &operator=(const Vector2<T> &rhs);

		Vector2<T> &operator+=(const Vector2<T> &rhs);
		Vector2<T> &operator-=(const Vector2<T> &rhs);
		Vector2<T> &operator*=(const Vector2<T> &rhs);
		Vector2<T> &operator/=(const Vector2<T> &rhs);
		Vector2<T> &operator*=(T scalar);
		Vector2<T> &operator/=(T scalar);

		const Vector2<T> operator+(const Vector2<T> &other) const;
		const Vector2<T> operator-(const Vector2<T> &other) const;
		const Vector2<T> operator*(const Vector2<T> &other) const;
		const Vector2<T> operator/(const Vector2<T> &other) const;
		const Vector2<T> operator*(T scalar) const;
		const Vector2<T> operator/(T scalar) const;

		bool operator==(const Vector2<T> &other) const;
		bool operator!=(const Vector2<T> &other) const;

		float Dot(const Vector2<T> &other) const;
		float Cross(const Vector2<T> &other) const;

		void Normalize();

		float GetLength() const;
		T GetLengthSquared() const;

		T x;
		T y;
	};

	typedef Vector2<int> Vector2i;
	typedef Vector2<float> Vector2f;

	template<typename T>
	Vector2<T> &Vector2<T>::operator=(const Vector2<T> &rhs)
	{
		if (this != &rhs)
		{
			x = rhs.x;
			y = rhs.y;
		}

		return *this;
	}

	template<typename T>
	Vector2<T> &Vector2<T>::operator+=(const Vector2<T> &rhs)
	{
		x += rhs.x;
		y += rhs.y;

		return *this;
	}
	template<typename T>
	Vector2<T> &Vector2<T>::operator-=(const Vector2<T> &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;

		return *this;
	}
	template<typename T>
	Vector2<T> &Vector2<T>::operator*=(const Vector2<T> &rhs)
	{
		x *= rhs.x;
		y *= rhs.y;

		return *this;
	}
	template<typename T>
	Vector2<T> &Vector2<T>::operator/=(const Vector2<T> &rhs)
	{
		x /= rhs.x;
		y /= rhs.y;

		return *this;
	}
	template<typename T>
	Vector2<T> &Vector2<T>::operator*=(T scalar)
	{
		x *= scalar;
		y *= scalar;

		return *this;
	}
	template<typename T>
	Vector2<T> &Vector2<T>::operator/=(T scalar)
	{
		x /= scalar;
		y /= scalar;

		return *this;
	}

	template<typename T>
	const Vector2<T> Vector2<T>::operator+(const Vector2<T> &other) const
	{
		return Vector2<T>(*this) += other;
	}
	template<typename T>
	const Vector2<T> Vector2<T>::operator-(const Vector2<T> &other) const
	{
		return Vector2<T>(*this) -= other;
	}
	template<typename T>
	const Vector2<T> Vector2<T>::operator*(const Vector2<T> &other) const
	{
		return Vector2<T>(*this) *= other;
	}
	template<typename T>
	const Vector2<T> Vector2<T>::operator/(const Vector2<T> &other) const
	{
		return Vector2<T>(*this) /= other;
	}
	template<typename T>
	const Vector2<T> Vector2<T>::operator*(T scalar) const
	{
		return Vector2<T>(*this) *= scalar;
	}
	template<typename T>
	const Vector2<T> Vector2<T>::operator/(T scalar) const
	{
		return Vector2<T>(*this) /= scalar;
	}

	template<typename T>
	const Vector2<T> operator*(T scalar, const Vector2<T> &other)
	{
		return Vector2<T>(other) *= scalar;
	}
	template<typename T>
	const Vector2<T> operator/(T scalar, const Vector2<T> &other)
	{
		return Vector2<T>(other) /= scalar;
	}

	template<typename T>
	bool Vector2<T>::operator==(const Vector2<T> &other) const
	{
		return ((x == other.x)&&(y == other.y));
	}
	template<typename T>
	bool Vector2<T>::operator!=(const Vector2<T> &other) const
	{
		return !(*this == other);
	}

	template<typename T>
	float Vector2<T>::Dot(const Vector2<T> &other) const
	{
		return x * other.x + y * other.y;
	}
	template<typename T>
	float Vector2<T>::Cross(const Vector2<T> &other) const
	{
		return x * other.y - y * other.x;
	}

	template<typename T>
	void Vector2<T>::Normalize()
	{
		const float length = GetLength();
		if (length > 0.f)
			(*this) /= length;
	}

	template<typename T>
	float Vector2<T>::GetLength() const
	{
		return Math::Sqrt(GetLengthSquared());
	}
	template<typename T>
	T Vector2<T>::GetLengthSquared() const
	{
		return (x*x + y*y);
	}
}

#endif // PZ_Vector2_h__