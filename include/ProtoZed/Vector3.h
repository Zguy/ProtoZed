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
#ifndef PZ_Vector3_h__
#define PZ_Vector3_h__

namespace PZ
{
	template<typename T>
	class Vector3
	{
	public:
		Vector3() : x(0), y(0), z(0)
		{}
		Vector3(const Vector3<T> &other) : x(other.x), y(other.y), z(other.z)
		{}
		Vector3(T x, T y, T z) : x(x), y(y), z(z)
		{}
		~Vector3()
		{}

		Vector3<T> &operator=(const Vector3<T> &rhs);

		Vector3<T> &operator+=(const Vector3<T> &rhs);
		Vector3<T> &operator-=(const Vector3<T> &rhs);
		Vector3<T> &operator*=(const Vector3<T> &rhs);
		Vector3<T> &operator/=(const Vector3<T> &rhs);
		Vector3<T> &operator*=(T scalar);
		Vector3<T> &operator/=(T scalar);

		const Vector3<T> operator+(const Vector3<T> &other) const;
		const Vector3<T> operator-(const Vector3<T> &other) const;
		const Vector3<T> operator*(const Vector3<T> &other) const;
		const Vector3<T> operator/(const Vector3<T> &other) const;
		const Vector3<T> operator*(T scalar) const;
		const Vector3<T> operator/(T scalar) const;

		bool operator==(const Vector3<T> &other) const;
		bool operator!=(const Vector3<T> &other) const;

		float Dot(const Vector3<T> &other) const;
		Vector3<T> Cross(const Vector3<T> &other) const;

		void Normalize();

		float GetLength() const;
		T GetLengthSquared() const;

		T x;
		T y;
		T z;
	};

	typedef Vector3<int> Vector3i;
	typedef Vector3<float> Vector3f;

	template<typename T>
	Vector3<T> &Vector3<T>::operator=(const Vector3<T> &rhs)
	{
		if (this != &rhs)
		{
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
		}

		return *this;
	}

	template<typename T>
	Vector3<T> &Vector3<T>::operator+=(const Vector3<T> &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;

		return *this;
	}
	template<typename T>
	Vector3<T> &Vector3<T>::operator-=(const Vector3<T> &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;

		return *this;
	}
	template<typename T>
	Vector3<T> &Vector3<T>::operator*=(const Vector3<T> &rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;

		return *this;
	}
	template<typename T>
	Vector3<T> &Vector3<T>::operator/=(const Vector3<T> &rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;

		return *this;
	}
	template<typename T>
	Vector3<T> &Vector3<T>::operator*=(T scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;

		return *this;
	}
	template<typename T>
	Vector3<T> &Vector3<T>::operator/=(T scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;

		return *this;
	}

	template<typename T>
	const Vector3<T> Vector3<T>::operator+(const Vector3<T> &other) const
	{
		return Vector3<T>(*this) += other;
	}
	template<typename T>
	const Vector3<T> Vector3<T>::operator-(const Vector3<T> &other) const
	{
		return Vector3<T>(*this) -= other;
	}
	template<typename T>
	const Vector3<T> Vector3<T>::operator*(const Vector3<T> &other) const
	{
		return Vector3<T>(*this) *= other;
	}
	template<typename T>
	const Vector3<T> Vector3<T>::operator/(const Vector3<T> &other) const
	{
		return Vector3<T>(*this) /= other;
	}
	template<typename T>
	const Vector3<T> Vector3<T>::operator*(T scalar) const
	{
		return Vector3<T>(*this) *= scalar;
	}
	template<typename T>
	const Vector3<T> Vector3<T>::operator/(T scalar) const
	{
		return Vector3<T>(*this) /= scalar;
	}

	template<typename T>
	const Vector3<T> operator*(T scalar, const Vector3<T> &other)
	{
		return Vector3<T>(other) *= scalar;
	}
	template<typename T>
	const Vector3<T> operator/(T scalar, const Vector3<T> &other)
	{
		return Vector3<T>(other) /= scalar;
	}

	template<typename T>
	bool Vector3<T>::operator==(const Vector3<T> &other) const
	{
		return ((x == other.x)&&(y == other.y)&&(z == other.z));
	}
	template<typename T>
	bool Vector3<T>::operator!=(const Vector3<T> &other) const
	{
		return !(*this == other);
	}

	template<typename T>
	float Vector3<T>::Dot(const Vector3<T> &other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}
	template<typename T>
	Vector3<T> Vector3<T>::Cross(const Vector3<T> &other) const
	{
		return Vector3<T>(
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
		);
	}

	template<typename T>
	void Vector3<T>::Normalize()
	{
		const float length = GetLength();
		if (length > 0.f)
			(*this) /= length;
	}

	template<typename T>
	float Vector3<T>::GetLength() const
	{
		return std::sqrt(GetLengthSquared());
	}
	template<typename T>
	T Vector3<T>::GetLengthSquared() const
	{
		return (x*x + y*y + z*z);
	}
}

#endif // PZ_Vector3_h__