#pragma once
#include <stdio.h>
namespace Math
{
	class Vector3;
	class Integer3
	{
	public:
		int x, y, z;
		Integer3() : x(0), y(0), z(0) {}
		Integer3(const Integer3& in) : x(in.x), y(in.y), z(in.z) {}
		Integer3(const Vector3& in);
		Integer3(const int& a, const int& b, const int& c) : x(a), y(b), z(c) {}

		Integer3 operator+(const Integer3& a);

		// Return a new vector wich is the substraction of 'a' and 'b'
		Integer3 operator-(const Integer3& a);

		// Return the result of the aritmetic multiplication of 'a' and 'b'
		Integer3 operator*(const Integer3& a);

		// Return the result of the aritmetic multiplication of 'a' and 'b'
		Integer3 operator*(const float& a);

		// Return the result of the aritmetic division of 'a' and 'b', will return 'a' * VEC2D_HIGH_VALUE if 'b' is equal 0;
		Integer3 operator/(const float& b);

		bool operator==(const Integer3& b);

		int& operator[](const size_t a);

		void Print();
	};
}