#pragma once
#include <string>
namespace Math {
	class Vector3;
	class Vector4
	{
	public:
		float x;
		float y;
		float z;
		float w;

		// Return a new empty Vector4
		inline Vector4() : x(0), y(0), z(0), w(0) {}

		inline Vector4(float a) : x(a), y(a), z(a), w(a) {}

		// Return a new Vector4 initialised with 'a', 'b', 'c' and 'd'
		inline Vector4(float a, float b, float c, float d = 1) : x(a), y(b), z(c), w(d) {}

		// Return a new Vector4 initialised with 'in'
		inline Vector4(const Vector3& in, float wIn = 1.0f);

		// Return a new Vector4 initialised with 'in'
		inline Vector4(const Vector4& in) : x(in.x), y(in.y), z(in.z), w(in.w) {}

		// Print the Vector4
		void Print();

		std::string ToString();

		// Return the Vec3D of Vector4
		inline Vector3 GetVector() const;

		// Return the length squared
		inline float LengthSquared() const;

		// Return the length
		inline float GetLength() const;

		// Divide each components by w, or set to VEC_HIGH_VALUE if w equals 0
		inline Vector4 Homogenize() const;

		inline Vector4 operator+(const Vector4& a) const;

		inline Vector4 operator-(const Vector4& a) const;

		inline Vector4 operator*(const Vector4& a) const;

		inline Vector4 operator*(const float& a) const;

		inline Vector4 operator/(const float& b) const;

		inline bool operator==(const Vector4& b);

		inline float& operator[](const size_t a);
		inline const float& operator[](const size_t a) const;

		// Return tue if 'a' and 'b' are collinears (Precision defined by VEC_COLLINEAR_PRECISION)
		inline bool IsCollinearWith(Vector4 a);

		inline float DotProduct(Vector4 a);

		// Return the z component of the cross product of 'a' and 'b'
		inline Vector4 CrossProduct(Vector4 a);

		// Return a vector with the same direction that 'in', but with length 1
		inline Vector4 UnitVector();

		// Return a vector of length 'in' and with an opposite direction
		inline Vector4 Negate();

		// return true if 'a' converted to int is equivalent to 'in' converted to int
		inline bool IsIntEquivalent(Vector4 a);
	};
}