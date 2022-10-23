#pragma once
#include <string>
namespace Math {
	class Vector4;
	class Vector3;
	class Quaternion;
	class Matrix4
	{
	public:
		/* data of the matrix : content[y][x]
		 * Matrix is indexed with:
		 *
		 * 00 | 01 | 02 | 03
		 * 10 | 11 | 12 | 13
		 * 20 | 21 | 22 | 23
		 * 30 | 31 | 32 | 33
		 *
		*/
		float content[4][4] = { 0 };

		Matrix4() {}

		Matrix4(float diagonal);

		Matrix4(const Matrix4& in);

		Matrix4(const float* data);

		Matrix4 Multiply(Matrix4 a) const;

		Matrix4 operator*(const Matrix4& a) const;

		inline Vector4 operator*(const Vector4& a) const;

		Matrix4 operator+(const Matrix4& a) const;

		static Matrix4 Identity();

		inline static Matrix4 CreateTransformMatrix(const Vector3& position, const Vector3& rotation, const Vector3& scale);

		inline static Matrix4 CreateTranslationMatrix(const Vector3& translation);

		inline static Matrix4 CreateScaleMatrix(const Vector3& scale);

		inline static Matrix4 CreateXRotationMatrix(float angle);

		inline static Matrix4 CreateYRotationMatrix(float angle);

		inline static Matrix4 CreateZRotationMatrix(float angle);

		inline static Matrix4 CreateRotationMatrix(const Vector3& rotation);

		inline static Matrix4 CreatePerspectiveProjectionMatrix(float, float, float);

		inline const Quaternion ToQuaternion();

		inline Vector3 GetPosition();

		inline Vector3 GetRotation();

		inline Vector3 GetScale();

		Matrix4 CreateInverseMatrix();

		Matrix4 CreateAdjMatrix();

		Matrix4 GetCofactor(int p, int q, int n);

		int GetDeterminant(int n);

		Matrix4 TransposeMatrix();

		inline float& operator[](const size_t a);

		inline float& at(const unsigned char x, const unsigned char y);

		void PrintMatrix();

		std::string ToString()
		{
			std::string print;
			for (int j = 0; j < 4; j++)
			{
				print += "{";
				for (int i = 0; i < 4; i++)
				{
					print += " ";
					print += std::to_string(content[j][i]);
				}
				print += "}";
			}
			return print;
		}

	};
}