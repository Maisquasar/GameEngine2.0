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

		inline Quaternion operator+(const Quaternion& a);

		inline Quaternion operator-(const Quaternion& a);

		inline Quaternion operator*(const Quaternion& a);

		static Quaternion Identity() { return Quaternion(0, 0, 0, 1); }

		inline static Quaternion AngleAxis(float angle, Vector3 axis);

		void Inverse();

		Quaternion GetInverse();

		void Normalize();

		Quaternion GetNormalized();

		float Dot(Quaternion a);

		inline Vector3 ToEuler();

		inline Matrix4 ToRotationMatrix();

		void Print() const;
	};
}