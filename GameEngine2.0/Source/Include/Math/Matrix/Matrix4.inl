#include "../MathFunction.h"
#include "../Matrix/Matrix4.h"
#include "../Matrix/Quaternion.h"
#include "../Vector/Vector4.h"
#include "../Vector/Vector3.h"

using namespace Math;

inline float& Matrix4::operator[](const size_t in)
{
	return content[in / 4][in % 4];
}

inline float& Matrix4::at(const unsigned char x, const unsigned char y)
{
	if (x > 3 || y > 3)
		return content[0][0];
	return content[y][x];
}

inline Vector4 Matrix4::operator*(const Vector4& in) const
{
	Vector4 out;
	for (size_t i = 0; i < 4; i++)
	{
		float res = 0;
		for (size_t k = 0; k < 4; k++) res += content[i][k] * in[k];
		out[i] = res;
	}
	return out;
}

inline Matrix4 Matrix4::CreateXRotationMatrix(float angle)
{
	Matrix4 out = Matrix4(1);
	float radA = ToRadians(angle);
	float cosA = cosf(radA);
	float sinA = sinf(radA);
	out.at(1, 1) = cosA;
	out.at(2, 1) = -sinA;
	out.at(1, 2) = sinA;
	out.at(2, 2) = cosA;
	return out;
}

inline Matrix4 Matrix4::CreateYRotationMatrix(float angle)
{
	Matrix4 out = Matrix4(1);
	float radA = ToRadians(angle);
	float cosA = cosf(radA);
	float sinA = sinf(radA);
	out.at(0, 0) = cosA;
	out.at(2, 0) = sinA;
	out.at(0, 2) = -sinA;
	out.at(2, 2) = cosA;
	return out;
}

inline Matrix4 Matrix4::CreateZRotationMatrix(float angle)
{
	Matrix4 out = Matrix4(1);
	float radA = ToRadians(angle);
	float cosA = cosf(radA);
	float sinA = sinf(radA);
	out.at(0, 0) = cosA;
	out.at(1, 0) = -sinA;
	out.at(0, 1) = sinA;
	out.at(1, 1) = cosA;
	return out;
}

inline Matrix4 Matrix4::CreateTranslationMatrix(const Vector3& translation)
{
	Matrix4 out = Matrix4(1);
	for (size_t i = 0; i < 3; i++) out.at(3, (const unsigned char)i) = translation[i];
	return out;
}

inline Matrix4 Matrix4::CreateRotationMatrix(const Vector3& rotation)
{
	return CreateYRotationMatrix(rotation.y) * CreateXRotationMatrix(rotation.x) * CreateZRotationMatrix(rotation.z);
}

inline Matrix4 Matrix4::CreateScaleMatrix(const Vector3& scale)
{
	Matrix4 out;
	for (size_t i = 0; i < 3; i++) out.at((const unsigned char)i, (const unsigned char)i) = scale[i];
	out.content[3][3] = 1;
	return out;
}

inline Matrix4 Matrix4::CreatePerspectiveProjectionMatrix(float Near, float Far, float fov)
{
	float s = 1.0f / tanf(ToRadians(fov / 2.0f));
	float param1 = -Far / (Far - Near);
	float param2 = param1 * Near;
	Matrix4 out;
	out.at(0, 0) = s;
	out.at(1, 1) = s;
	out.at(2, 2) = param1;
	out.at(3, 2) = -1;
	out.at(2, 3) = param2;
	return out;
}

inline Matrix4 Matrix4::CreateTransformMatrix(const Vector3& position, const Vector3& rotation, const Vector3& scale)
{
	return CreateTranslationMatrix(position) * CreateYRotationMatrix(rotation.y) * CreateXRotationMatrix(rotation.x) * CreateZRotationMatrix(rotation.z) * CreateScaleMatrix(scale);
}

inline Vector3 Matrix4::GetPosition()
{
	return Vector3(content[0][3], content[1][3], content[2][3]);
}

inline Vector3 Matrix4::GetRotation()
{
	Vector3 sca = GetScale();
	Matrix4 rotMat;
	// Get Rotation Matrix.
	if (sca == Vector3(0))
		sca = Vector3(0.0001f);
	rotMat.at(0, 0) = at(0, 0) / sca.x;
	rotMat.at(0, 1) = at(0, 1) / sca.x;
	rotMat.at(0, 2) = at(0, 2) / sca.x;
	rotMat.at(1, 0) = at(1, 0) / sca.y;
	rotMat.at(1, 1) = at(1, 1) / sca.y;
	rotMat.at(1, 2) = at(1, 2) / sca.y;
	rotMat.at(2, 0) = at(2, 0) / sca.z;
	rotMat.at(2, 1) = at(2, 1) / sca.z;
	rotMat.at(2, 2) = at(2, 2) / sca.z;
	rotMat.at(3, 3) = 1;

	// Get Rotation from rotation matrix.
	float thetaX, thetaY, thetaZ;
	if (rotMat.at(2, 1) < 1)
	{
		if (rotMat.at(2, 1) > -1)
		{
			thetaX = asin(-rotMat.at(2, 1));
			thetaY = atan2(rotMat.at(2, 0), rotMat.at(2, 2));
			thetaZ = atan2(rotMat.at(0, 1), rotMat.at(1, 1));
		}
		else
		{
			thetaX = (float)PI / 2;
			thetaY = -atan2(rotMat.at(1, 0), rotMat.at(0, 0));
			thetaZ = 0;
		}
	}
	else
	{
		thetaX = -PI / 2;
		thetaY = atan2(rotMat.at(1, 0), rotMat.at(0, 0));
		thetaZ = 0;
	}
	return Vector3(thetaX, thetaY, thetaZ).ToDeegree();
}

inline Vector3 Matrix4::GetScale()
{
	// World Scale equal lenght of columns of the model matrix.
	float x = Vector3(content[0][0], content[0][1], content[0][2]).GetLength();
	float y = Vector3(content[1][0], content[1][1], content[1][2]).GetLength();
	float z = Vector3(content[2][0], content[2][1], content[2][2]).GetLength();
	return Vector3(x, y, z);
}

inline const Quaternion Matrix4::ToQuaternion()
{
	float w = sqrtf(1 + at(0, 0) + at(1, 1) + at(2, 2)) / 2;
	return Quaternion((at(2, 1) - at(1, 2)) / (4 * w), (at(0, 2) - at(2, 0)) / (4 * w), (at(1, 0) - at(0, 1)) / (4 * w), w);
}