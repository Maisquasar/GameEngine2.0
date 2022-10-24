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
		Vector4() : x(0), y(0), z(0), w(0) {}

		Vector4(float a) : x(a), y(a), z(a), w(a) {}

		// Return a new Vector4 initialised with 'a', 'b', 'c' and 'd'
		Vector4(float a, float b, float c, float d = 1) : x(a), y(b), z(c), w(d) {}

		// Return a new Vector4 initialised with 'in'
		Vector4(const Vector3& in, float wIn = 1.0f);

		// Return a new Vector4 initialised with 'in'
		Vector4(const Vector4& in) : x(in.x), y(in.y), z(in.z), w(in.w) {}

		// Print the Vector4
		void Print();

		std::string ToString();

		// Return the Vec3D of Vector4
		Vector3 GetVector() const;

		// Return the length squared
		float LengthSquared() const;

		// Return the length
		float GetLength() const;

		// Divide each components by w, or set to VEC_HIGH_VALUE if w equals 0
		Vector4 Homogenize() const;

		Vector4 operator+(const Vector4& a) const;

		Vector4 operator-(const Vector4& a) const;

		Vector4 operator*(const Vector4& a) const;

		Vector4 operator*(const float& a) const;

		Vector4 operator/(const float& b) const;

		bool operator==(const Vector4& b);

		float& operator[](const size_t a);
		const float& operator[](const size_t a) const;

		// Return tue if 'a' and 'b' are collinears (Precision defined by VEC_COLLINEAR_PRECISION)
		bool IsCollinearWith(Vector4 a);

		float DotProduct(Vector4 a);

		// Return the z component of the cross product of 'a' and 'b'
		Vector4 CrossProduct(Vector4 a);

		// Return a vector with the same direction that 'in', but with length 1
		Vector4 UnitVector();

		// Return a vector of length 'in' and with an opposite direction
		Vector4 Negate();

		// return true if 'a' converted to int is equivalent to 'in' converted to int
		bool IsIntEquivalent(Vector4 a);
	};
}