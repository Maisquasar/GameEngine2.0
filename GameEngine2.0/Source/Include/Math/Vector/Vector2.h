#pragma once
#include <string>
#define VEC_COLLINEAR_PRECISION 0.001f
#define VEC_HIGH_VALUE 1e20f


namespace Math {
	class Integer2;
	class Vector2
	{
	public:
		float x;
		float y;

		// Return a new empty Vec2D
		inline Vector2() : x(0), y(0) {}

		// Return a new Vector2 initialised with 'a' and 'b'
		inline Vector2(float a, float b) : x(a), y(b) {}

		// Return a new Vector2 initialised with 'in'
		inline Vector2(const Vector2& in) : x(in.x), y(in.y) {}
		inline Vector2(const Integer2& in);

		// Return the length squared of 'in'
		inline float LengthSquared();

		//Return the distance between 'in' an 'a'
		inline float GetDistanceFromPoint(Vector2 a);

		// Return the lenght of the given Vector
		inline float GetLength();

		// Return a new vector wich is the sum of 'a' and 'b'
		inline Vector2 operator+(const Vector2& a) const;

		// Return a new vector wich is the substraction of 'a' and 'b'
		inline Vector2 operator-(const Vector2& a) const;

		// Return the result of the aritmetic multiplication of 'a' and 'b'
		inline Vector2 operator*(const Vector2& a) const;

		// Return the result of the aritmetic multiplication of 'a' and 'b'
		inline Vector2 operator*(const float& a) const;

		// Return the result of the aritmetic division of 'a' and 'b', will return 'a' * VEC2D_HIGH_VALUE if 'b' is equal 0;
		inline Vector2 operator/(const float& b) const;

		inline bool operator==(const Vector2& b) const;

		// Return tue if 'a' and 'b' are collinears (Precision defined by VEC2D_COLLINEAR_PRECISION)
		inline bool IsCollinearWith(Vector2 a);

		// Return the dot product of 'a' and 'b'
		inline float DotProduct(Vector2 a);

		// Return the z component of the cross product of 'a' and 'b'
		inline float CrossProduct(Vector2 a);

		// Return a vector with the same direction that 'in', but with length 1
		inline Vector2 UnitVector();

		// Return a vector of length 'in' and with an opposite direction
		inline Vector2 Negate();

		// Return the normal vector of 'in'
		inline Vector2 GetNormal();

		// return true if 'a' converted to int is equivalent to 'in' converted to int
		inline bool IsIntEquivalent(Vector2 a);

		void Print();

		std::string ToString();

	private:

	};
}