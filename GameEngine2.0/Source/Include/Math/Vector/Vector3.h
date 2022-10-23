#pragma once
#include <string>
#include <cmath>
#define VEC_COLLINEAR_PRECISION 0.001f
#define VEC_HIGH_VALUE 1e20f


namespace Math {
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
		inline Vector3() : x(0), y(0), z(0) {}

		inline Vector3(float content) : x(content), y(content), z(content) {}

		inline Vector3(Vector4 a);

		// Return a new Vector3 initialised with 'a' and 'b'
		inline Vector3(float a, float b, float c) : x(a), y(b), z(c) {}

		// Return a new Vector3 initialised with 'in'
		inline Vector3(const Vector3& in) : x(in.x), y(in.y), z(in.z) {}

		// Print the Vector3
		void Print() const;

		// Return the length squared of 'in'
		inline float LengthSquared() const;

		// Return the lenght of the given Vector
		inline float GetLength() const;

		inline Vector3 operator=(const Vector4& b) const;

		inline Vector3 operator=(Vector4 b);

		inline Vector3 operator=(Integer3 b);

		inline Vector3 operator=(const float b[3]);

		// Return a new vector wich is the sum of 'a' and 'b'
		inline Vector3 operator+(const Vector3& a) const;

		inline void operator+=(const Vector3& a);

		inline void operator-=(const Vector3& a);

		// Return a new vector wich is the substraction of 'a' and 'b'
		inline Vector3 operator-(const Vector3& a) const;

		// Return the result of the aritmetic multiplication of 'a' and 'b'
		inline Vector3 operator*(const Vector3& a) const;

		// Return the result of the aritmetic multiplication of 'a' and 'b'
		inline Vector3 operator*(const Integer3& a) const;

		// Return the result of the aritmetic multiplication of 'a' and 'b'
		inline Vector3 operator*(const float& a) const;

		// Return the result of the aritmetic division of 'a' and 'b', will return 'a' * VEC_HIGH_VALUE if 'b' is equal 0;
		inline Vector3 operator/(const float& b) const;

		inline bool operator==(const Vector3& b) const;

		inline bool operator>(const Vector3& b) const;

		inline bool operator<(const Vector3& b) const;

		inline bool operator!=(const Vector3& b) const;

		inline const float& operator[](const size_t a) const;

		inline float& operator[](const size_t a);

		// Return tue if 'a' and 'b' are collinears (Precision defined by VEC_COLLINEAR_PRECISION)
		inline bool IsCollinearWith(Vector3 a);

		inline float GetDistanceBetween(Vector3 a);

		// Return the dot product of 'a' and 'b'
		inline float DotProduct(Vector3 a);

		// Return the z component of the cross product of 'a' and 'b'
		inline Vector3 CrossProduct(Vector3 a);

		// Return a vector with the same direction that 'in', but with length 1
		inline Vector3 UnitVector() const;

		// Return a vector of length 'in' and with an opposite direction
		inline Vector3 Negate();

		// return true if 'a' converted to int is equivalent to 'in' converted to int
		inline bool IsIntEquivalent(Vector3 a);

		// Convert degree to rad
		inline Vector3 ToRad();

		// Convert rad to Degree
		inline Vector3 ToDeegree();

		inline Vector3 Clamp(Vector3 min, Vector3 max);

		//
		static inline Vector3 ClampMagnitude(Vector3, float);

		Vector3 Normalize()
		{
			float len = std::sqrt(x * x + y * y + z * z);

			if (len != 0.)
			{

				return Vector3(x / len, y / len, z / len);
			}
			return Vector3(x, y, z);
		}

		inline Quaternion ToQuaternion();

		std::string ToString();
	};
}