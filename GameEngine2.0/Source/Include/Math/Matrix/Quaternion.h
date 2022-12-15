#pragma once
namespace Math {
	class Vector3;
	class Vector4;
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
		Quaternion(Vector4 a) : x(a.x), y(a.y), z(a.z), w(a.w) {}
		~Quaternion() {}

		Quaternion operator+(const Quaternion& a);

		Quaternion operator-(const Quaternion& a);

		Quaternion operator*(const Quaternion& a);

		Quaternion operator*(const float& a);

		Vector3 operator*(const Vector3& a);

		float& operator[](int index);

		static Quaternion Identity() { return Quaternion(0, 0, 0, 1); }

		static Quaternion AngleAxis(float angle, Vector3 axis);

		static Quaternion LookRotation(Vector3 forward, Vector3 up);

		static Quaternion SLerp(Quaternion a, Quaternion b, float time);

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

		std::string ToString();
	};
}