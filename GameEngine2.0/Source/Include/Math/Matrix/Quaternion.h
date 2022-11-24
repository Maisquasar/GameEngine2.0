#pragma once
namespace Math {
	class Vector3;
	class Matrix4;
	class Quaternion
	{
	public:
		float x;
		float y;
		float z;
		float w;

		Quaternion() : x(0), y(0), z(0), w(1) {}
		Quaternion(float a) : x(a), y(a), z(a), w(a) {}
		Quaternion(float a, float b, float c, float d = 1) : x(a), y(b), z(c), w(d) {}
		~Quaternion() {}

		Quaternion operator+(const Quaternion& a);

		Quaternion operator-(const Quaternion& a);

		Quaternion operator*(const Quaternion& a);

		Vector3 operator*(const Vector3& a);

		static Quaternion Identity() { return Quaternion(0, 0, 0, 1); }

		static Quaternion AngleAxis(float angle, Vector3 axis);

		void Inverse();

		Quaternion GetInverse();

		void Normalize();

		Quaternion GetNormalized();

		void Conjugate();

		Quaternion GetConjugate();

		float Dot(Quaternion a);

		Vector3 ToEuler();

		Matrix4 ToRotationMatrix();

		void Print() const;
	};
}