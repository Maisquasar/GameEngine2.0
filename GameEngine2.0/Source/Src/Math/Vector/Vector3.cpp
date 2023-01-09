#include "Include/Math/Math.h"
using namespace Math;

Vector3::Vector3(Vector2 a) : x(a.x), y(a.y) {}

Vector3::Vector3(Vector4 a) : x(a.x), y(a.y), z(a.z) {}

float Vector3::LengthSquared() const
{
	return (x * x + y * y + z * z);
}

float Vector3::GetLength() const
{
	return sqrtf(LengthSquared());
}

Vector3 Vector3::operator=(const Vector4& b) const
{
	return Vector3(b.x, b.y, b.z);
}
Vector3 Vector3::operator=(Vector4 b)
{
	return Vector3(b.x, b.y, b.z);
}
Vector3 Vector3::operator=(Integer3 b)
{
	return Vector3((float)b.x, (float)b.y, (float)b.z);
}

Vector3 Vector3::operator=(const float b[3])
{
	return Vector3(b[0], b[1], b[2]);
}

Vector3 Vector3::operator+(const Vector3& a) const
{
	Vector3 res = Vector3(a.x + this->x, a.y + this->y, a.z + this->z);
	return res;
}

void Vector3::operator+=(const Vector3& a)
{
	this->x += a.x;
	this->y += a.y;
	this->z += a.z;
}

void Vector3::operator-=(const Vector3& a)
{
	this->x -= a.x;
	this->y -= a.y;
	this->z -= a.z;
}

Vector3 Vector3::operator-(const Vector3& a) const
{
	Vector3 res = Vector3(this->x - a.x, this->y - a.y, this->z - a.z);
	return res;
}

Vector3 Vector3::operator*(const Vector3& a) const
{
	return { x * a.x, y * a.y, z * a.z };
}

Vector3 Vector3::operator*(const Integer3& a) const
{
	return { x * a.x, y * a.y, z * a.z };
}

Vector3 Vector3::operator*(const float& a) const
{
	Vector3 res = Vector3(this->x * a, this->y * a, this->z * a);
	return res;
}

Vector3 Vector3::operator/(const float& a) const
{
	if (a == 0.0f)
		return operator*(VEC_HIGH_VALUE);
	Vector3 res = operator*(1 / a);
	return res;
}

Vector3 Math::Vector3::operator/(const Vector3& other) const
{
	return Vector3(x / other.x, y / other.y, z / other.z);
}

bool Vector3::operator==(const Vector3& b) const
{
	return (x == b.x && y == b.y && z == b.z);
}

bool Vector3::operator>(const Vector3& b) const
{
	return (x > b.x || y > b.y || z > b.z);
}

bool Vector3::operator<(const Vector3& b) const
{
	return (x < b.x || y < b.y || z < b.z);
}

bool Vector3::operator!=(const Vector3& b) const
{
	return (x != b.x || y != b.y || z != b.z);
}

float& Vector3::operator[](const size_t a)
{
	return *((&x) + a);
}

const float& Vector3::operator[](const size_t a) const
{
	return *((&x) + a);
}

float Vector3::DotProduct(Vector3 a)
{
	return (a.x * x + a.y * y + a.z * z);
}

bool Vector3::IsCollinearWith(Vector3 a)
{
	float res = this->DotProduct(a);
	return (res < VEC_COLLINEAR_PRECISION);
}

float Vector3::GetDistanceBetween(Vector3 a)
{
	float i = a.x - x;
	float j = a.y - y;
	float h = a.z - z;
	return sqrt(i * i + j * j + h * h);
}


Vector3 Vector3::CrossProduct(Vector3 a)
{
	return Vector3((y * a.z) - (z * a.y), (z * a.x) - (x * a.z), (x * a.y) - (y * a.x));
}

Vector3 Vector3::UnitVector() const
{
	return operator/(GetLength());
}

Vector3 Vector3::Negate()
{
	return operator*(-1);
}

bool Vector3::IsIntEquivalent(Vector3 a)
{
	return ((int)x == (int)a.x && (int)y == a.y && (int)z == (int)a.z);
}


// Convert degree to rad
Vector3 Vector3::ToRad()
{
	return Vector3(ToRadians(x), ToRadians(y), ToRadians(z));
}

// Convert rad to Degree
Vector3 Vector3::ToDeegree()
{
	return Vector3(ToDegrees(x), ToDegrees(y), ToDegrees(z));
}

Vector3 Vector3::Clamp(Vector3 min, Vector3 max)
{
	Vector3 tmp = *this;
	if (tmp < min)
		tmp = min;
	else if (tmp > max)
		tmp = max;
	return tmp;
}

Vector3 Vector3::ClampMagnitude(Vector3 vector, float maxLength)
{
	float sqrmag = vector.LengthSquared();
	if (sqrmag > maxLength * maxLength)
	{
		float mag = (float)sqrtf(sqrmag);
		float normalized_x = vector.x / mag;
		float normalized_y = vector.y / mag;
		float normalized_z = vector.z / mag;
		return Vector3(normalized_x * maxLength,
			normalized_y * maxLength,
			normalized_z * maxLength);
	}
	return vector;
}

Vector3 Math::Vector3::SLerp(Math::Vector3 a, Math::Vector3 b, float t)
{
	float dot = a.DotProduct(b);

	if (dot > 1)
		dot = 1;
	else if (dot < -1)
		dot = -1;

	float theta = std::acos(dot) * t;
	Vector3 RelativeVec = b - a * dot;
	RelativeVec.Normalize();

	return ((a * std::cos(theta)) + (RelativeVec * std::sin(theta)));
}

Vector3 Math::Vector3::Lerp(Math::Vector3 a, Math::Vector3 b, float t)
{
	if (t < 0)
		return a;
	else if (t >= 1)
		return b;
	return a * (1 - t) + b * t;
}

Quaternion  Vector3::ToQuaternion()
{
	auto a = this->ToRad();
	// Calculate the cosine of each half angle
	float cy = std::cosf(a.z * 0.5f);
	float cp = std::cosf(a.y * 0.5f);
	float cr = std::cosf(a.x * 0.5f);

	// Calculate the sine of each half angle
	float sy = std::sinf(a.z * 0.5f);
	float sp = std::sinf(a.y * 0.5f);
	float sr = std::sinf(a.x * 0.5f);

	// Calculate the quaternion elements
	Quaternion q;
	q.w = cy * cp * cr + sy * sp * sr;
	q.x = cy * cp * sr - sy * sp * cr;
	q.y = sy * cp * sr + cy * sp * cr;
	q.z = sy * cp * cr - cy * sp * sr;

	return q;
}

void Math::Vector3::Print() const
{
	printf("(%.5f, %.5f, %.5f)\n", x, y, z);
}
std::string Math::Vector3::ToString()
{
	return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z);
}
