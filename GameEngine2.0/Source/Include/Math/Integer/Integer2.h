#pragma once
namespace Math {
	class Vector2;
	class Integer2
	{
	public:
		int x, y;
		inline Integer2() : x(0), y(0) {}
		inline Integer2(const Integer2& in) : x(in.x), y(in.y) {}
		inline Integer2(const Vector2& in);
		inline Integer2(const int& a, const int& b) : x(a), y(b) {}

		inline Integer2 operator+(const Integer2& a);

		// Return a new vector wich is the substraction of 'a' and 'b'
		inline Integer2 operator-(const Integer2& a);

		// Return the result of the aritmetic multiplication of 'a' and 'b'
		inline Integer2 operator*(const Integer2& a);

		// Return the result of the aritmetic multiplication of 'a' and 'b'
		inline Integer2 operator*(const float& a);

		// Return the result of the aritmetic division of 'a' and 'b', will return 'a' * VEC2D_HIGH_VALUE if 'b' is equal 0;
		inline Integer2 operator/(const float& b);

		inline bool operator==(const Integer2& b);
	};
}