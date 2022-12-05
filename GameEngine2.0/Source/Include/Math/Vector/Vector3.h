#pragma once
#include <string>
#include <cmath>

#define VEC_COLLINEAR_PRECISION 0.001f
#define VEC_HIGH_VALUE 1e20f


namespace Math {
	class Vector2;
	class Vector4;
	class Integer3;
	class Quaternion;
	class Vector3
	{
	public:
		float x;
		float y;
		float z;

		// Return a new empty Vector3
		Vector3() : x(0), y(0), z(0) {}

		Vector3(float content) : x(content), y(content), z(content) {}

		Vector3(Vector2 a);

		Vector3(Vector4 a);

		// Return a new Vector3 initialised with 'a' and 'b'
		Vector3(float a, float b, float c) : x(a), y(b), z(c) {}

		// Return a new Vector3 initialised with 'in'
		Vector3(const Vector3& in) : x(in.x), y(in.y), z(in.z) {}

		// Print the Vector3
		void Print() const;

		// Return the length squared of 'in'
		float LengthSquared() const;

		// Return the lenght of the given Vector
		float GetLength() const;

		Vector3 operator=(const Vector4& b) const;

		Vector3 operator=(Vector4 b);

		Vector3 operator=(Integer3 b);

		Vector3 operator=(const float b[3]);

		// Return a new vector wich is the sum of 'a' and 'b'
		Vector3 operator+(const Vector3& a) const;

		void operator+=(const Vector3& a);

		void operator-=(const Vector3& a);

		// Return a new vector wich is the substraction of 'a' and 'b'
		Vector3 operator-(const Vector3& a) const;

		// Return the result of the aritmetic multiplication of 'a' and 'b'
		Vector3 operator*(const Vector3& a) const;

		// Return the result of the aritmetic multiplication of 'a' and 'b'
		Vector3 operator*(const Integer3& a) const;

		// Return the result of the aritmetic multiplication of 'a' and 'b'
		Vector3 operator*(const float& a) const;

		// Return the result of the aritmetic division of 'a' and 'b', will return 'a' * VEC_HIGH_VALUE if 'b' is equal 0;
		Vector3 operator/(const float& b) const;

		Vector3 operator/(const Vector3& other) const;

		bool operator==(const Vector3& b) const;

		bool operator>(const Vector3& b) const;

		bool operator<(const Vector3& b) const;

		bool operator!=(const Vector3& b) const;

		const float& operator[](const size_t a) const;

		float& operator[](const size_t a);

		// Return tue if 'a' and 'b' are collinears (Precision defined by VEC_COLLINEAR_PRECISION)
		bool IsCollinearWith(Vector3 a);

		float GetDistanceBetween(Vector3 a);

		// Return the dot product of 'a' and 'b'
		float DotProduct(Vector3 a);

		// Return the z component of the cross product of 'a' and 'b'
		Vector3 CrossProduct(Vector3 a);

		// Return a vector with the same direction that 'in', but with length 1
		Vector3 UnitVector() const;

		// Return a vector of length 'in' and with an opposite direction
		Vector3 Negate();

		// return true if 'a' converted to int is equivalent to 'in' converted to int
		bool IsIntEquivalent(Vector3 a);

		// Convert degree to rad
		Vector3 ToRad();

		// Convert rad to Degree
		Vector3 ToDeegree();

		Vector3 Clamp(Vector3 min, Vector3 max);

		//
		static Vector3 ClampMagnitude(Vector3, float);

		static Vector3 Forward() { return Vector3(0, 0, 1); }

		static Vector3 Right() { return Vector3(1, 0, 0); }

		static Vector3 Up() { return Vector3(0, 1, 0); }

		Vector3 Normalize() const
		{
			float len = std::sqrt(x * x + y * y + z * z);

			if (len != 0.)
			{

				return Vector3(x / len, y / len, z / len);
			}
			return Vector3(x, y, z);
		}

		Quaternion ToQuaternion();

		std::string ToString();
	};
}