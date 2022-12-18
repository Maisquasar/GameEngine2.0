#include "Include/Math/Math.h"
using namespace Math;

Quaternion Quaternion::operator+(const Quaternion& a)
{
	return Quaternion(x + a.x, y + a.y, z + a.z, w + a.w);
}

Quaternion Quaternion::operator-(const Quaternion& a)
{
	return Quaternion(x - a.x, y - a.y, z - a.z, w - a.w);
}

Quaternion Quaternion::operator*(const Quaternion& a)
{
	return Quaternion(
		w * a.x + x * a.w + y * a.z - z * a.y,
		w * a.y + y * a.w + z * a.x - x * a.z,
		w * a.z + z * a.w + x * a.y - y * a.x,
		w * a.w - x * a.x - y * a.y - z * a.z);
}


Quaternion Math::Quaternion::operator*(const float& a)
{
	return Quaternion(this->x * a, this->y * a, this->z * a, this->w * a);
}

Vector3 Math::Quaternion::operator*(const Vector3& a)
{
	Vector3 vector;
	float ax = x * 2.f;
	float ay = y * 2.f;
	float az = z * 2.f;
	float xx = x * ax;
	float yy = y * ay;
	float zz = z * az;
	float xy = x * ay;
	float xz = x * az;
	float yz = y * az;
	float wx = w * ax;
	float wy = w * ay;
	float wz = w * az;
	vector.x = (1.f - (yy + zz)) * a.x + (xy - wz) * a.y + (xz + wy) * a.z;
	vector.y = (xy + wz) * a.x + (1.f - (xx + zz)) * a.y + (yz - wx) * a.z;
	vector.z = (xz - wy) * a.x + (yz + wx) * a.y + (1.f - (xx + yy)) * a.z;
	return vector;
}

float& Math::Quaternion::operator[](int index)
{
	return *((&x) + index);
}

Math::Quaternion Math::Quaternion::AngleAxis(float angle, Vector3 axis)
{
	Quaternion q;
	float rad = Math::ToRadians(angle);
	axis = axis.Normalize();
	q.w = cosf(rad / 2);
	q.x = sinf(rad / 2) * axis.x;
	q.y = sinf(rad / 2) * axis.y;
	q.z = sinf(rad / 2) * axis.z;
	return q;
}

Quaternion Math::Quaternion::LookRotation(Vector3 forward, Vector3 up)
{
	forward.Normalize();

	Vector3 vector = forward.Normalize();
	Vector3 vector2 = up.CrossProduct(vector).Normalize();
	Vector3 vector3 = vector.CrossProduct(vector2);
	float m00 = vector2.x;
	float m01 = vector2.y;
	float m02 = vector2.z;
	float m10 = vector3.x;
	float m11 = vector3.y;
	float m12 = vector3.z;
	float m20 = vector.x;
	float m21 = vector.y;
	float m22 = vector.z;


	float num8 = (m00 + m11) + m22;
	auto quaternion = Quaternion();
	if (num8 > 0.f)
	{
		float num = (float)sqrtf(num8 + 1.f);
		quaternion.w = num * 0.5f;
		num = 0.5f / num;
		quaternion.x = (m12 - m21) * num;
		quaternion.y = (m20 - m02) * num;
		quaternion.z = (m01 - m10) * num;
		return quaternion;
	}
	if ((m00 >= m11) && (m00 >= m22))
	{
		float num7 = (float)sqrtf(((1.f + m00) - m11) - m22);
		float num4 = 0.5f / num7;
		quaternion.x = 0.5f * num7;
		quaternion.y = (m01 + m10) * num4;
		quaternion.z = (m02 + m20) * num4;
		quaternion.w = (m12 - m21) * num4;
		return quaternion;
	}
	if (m11 > m22)
	{
		float num6 = (float)sqrtf(((1.f + m11) - m00) - m22);
		float num3 = 0.5f / num6;
		quaternion.x = (m10 + m01) * num3;
		quaternion.y = 0.5f * num6;
		quaternion.z = (m21 + m12) * num3;
		quaternion.w = (m20 - m02) * num3;
		return quaternion;
	}
	float num5 = (float)sqrtf(((1.f + m22) - m00) - m11);
	float num2 = 0.5f / num5;
	quaternion.x = (m20 + m02) * num2;
	quaternion.y = (m21 + m12) * num2;
	quaternion.z = 0.5f * num5;
	quaternion.w = (m01 - m10) * num2;
	return quaternion;
}

Quaternion Math::Quaternion::SLerp(Quaternion a, Quaternion b, float t)
{
	if (t < 0.0f)
		return a;
	else if (t >= 1.0f)
		return b;
	float d = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	float s0, s1;
	float sd = (float)((d > 0.0f) - (d < 0.0f));

	d = fabs(d);

	if (d < 0.9995f)
	{
		float s = sqrtf(1.0f - d * d);
		float a = atan2f(s, d);
		float c = cosf(t * a);


		s1 = sqrtf(1.0f - c * c) / s;
		s0 = c - d * s1;
	}
	else
	{
		s0 = 1.0f - t;
		s1 = t;
	}

	return a * s0 + b * sd * s1;
}

Math::Vector3 Math::Quaternion::ToEuler()
{
	// IT WORKS
	Math::Vector3 euler;
	Math::Matrix4 rotMat = ToRotationMatrix();

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
	if (thetaX == -0)
		thetaX = 0;
	if (thetaY == -0)
		thetaY = 0;
	if (thetaZ == -0)
		thetaZ = 0;
	return Vector3(thetaX, thetaY, thetaZ).ToDeegree();
}

Math::Matrix4 Math::Quaternion::ToRotationMatrix()
{
	Math::Matrix4 m = Math::Matrix4::Identity();
	m.at(0, 0) = 1 - 2 * pow(y, 2.f) - 2 * pow(z, 2.f);
	m.at(1, 0) = 2 * x * y + 2 * w * z;
	m.at(2, 0) = 2 * x * z + 2 * w * y;

	m.at(0, 1) = 2 * x * y + 2 * w * z;
	m.at(1, 1) = 1 - 2 * pow(x, 2.f) - 2 * pow(z, 2.f);
	m.at(2, 1) = 2 * y * z - 2 * w * x;

	m.at(0, 2) = 2 * x * z - 2 * w * y;
	m.at(1, 2) = 2 * y * z + 2 * w * x;
	m.at(2, 2) = 1 - 2 * pow(x, 2.f) - 2 * pow(y, 2.f);
	return m;
}

float Math::Quaternion::Dot(Math::Quaternion a)
{
	return x * a.x + y * a.y + z * a.z + w * a.w;
}

void Math::Quaternion::Normalize()
{
	*this = GetNormalized();
}

Math::Quaternion Math::Quaternion::GetNormalized()
{
	float mag = sqrtf(Dot(*this));

	if (mag < FLT_MIN)
		return Quaternion::Identity();
	else
		return Quaternion(x / mag, y / mag, z / mag, w / mag);
}

void Math::Quaternion::Conjugate()
{
	*this = GetConjugate();
}

Quaternion Math::Quaternion::GetConjugate()
{
	return Quaternion(-x, -y, -z, w);
}

void Math::Quaternion::Inverse()
{
	*this = GetInverse();
}

Math::Quaternion Math::Quaternion::GetInverse()
{
	float d = pow(w, 2.f) + pow(x, 2.f) + pow(y, 2.f) + pow(z, 2.f);
	if (x == 0 && y == 0 && z == 0 && w == 0)
		return *this;
	else
		return Quaternion(-x / d, -y / d, -z / d, w / d);


}

void Math::Quaternion::Print() const
{
	printf("Quaternion { %f, %f, %f, %f}\n", x, y, z, w);
}

std::string Math::Quaternion::ToString()
{
	return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + " " + std::to_string(w);
}
