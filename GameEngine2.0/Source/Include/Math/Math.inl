#include <string>

#include "Math.h"

namespace Math {

	inline float ToRadians(float in)
	{
		return in / 180.f * PI;
	}

	inline float ToDegrees(float in)
	{
		return in * 180.f / PI;
	}

	inline float Cut(float in, float min, float max)
	{
		if (in < min)
			in = min;
		if (in > max)
			in = max;
		return in;
	}

	inline int CutInt(int in, int min, int max)
	{
		if (in < min)
			in = min;
		if (in > max)
			in = max;
		return in;
	}

	inline float Mod(float in, float value)
	{
		return in - value * floorf(in / value);
	}

	inline float Min(float a, float b)
	{
		if (a > b)
			return b;
		return a;
	}

	inline float Max(float a, float b)
	{
		if (a > b)
			return a;
		return b;
	}

#pragma region Vector 2
	// ============== Vec2 Class ==============
	inline Vec2::Vec2(const IVec2& a) : x((float)a.x), y((float)a.y) {}

	inline Vec2 Vec2::operator+(const Vec2& a) const
	{
		return Vec2(a.x + this->x, a.y + this->y);
	}

	inline Vec2 Vec2::operator-(const Vec2& a) const
	{
		return Vec2(this->x - a.x, this->y - a.y);
	}

	inline Vec2 Vec2::operator-(void) const
	{
		return operator*(-1);
	}

	inline Vec2 Vec2::operator*(const Vec2& a)const
	{
		return Vec2(this->x * a.x, this->y * a.y);
	}

	inline Vec2 Vec2::operator*(const float& a)const
	{
		return Vec2(this->x * a, this->y * a);
	}

	inline Vec2 Vec2::operator/(const float& a)const
	{
		if (a == 0.0f)
			return Vec2();
		return operator*(1 / a);
	}

	inline bool Vec2::operator==(const Vec2& b)const
	{
		return (x == b.x && y == b.y);
	}

	inline bool Vec2::operator!=(const Vec2& b) const
	{
		return (x != b.x || y != b.y);
	}

	inline float& Vec2::operator[](const size_t a)
	{
		return *((&x) + a);
	}

	inline float Vec2::LengthSquared() const
	{
		return (x * x + y * y);
	}

	inline float Vec2::GetDistanceFromPoint(const Vec2& a) const
	{
		float dx = x - a.x;
		float dy = y - a.y;
		return std::sqrtf(dx * dx + dy * dy);
	}

	inline float Vec2::GetLength() const
	{
		return sqrtf(LengthSquared());
	}

	inline bool Vec2::IsCollinearWith(const Vec2& a) const
	{
		return std::abs(x * a.y - y * a.x) < std::numeric_limits<float>::epsilon();
	}

	inline float Vec2::DotProduct(const Vec2& a) const
	{
		return (a.x * x + a.y * y);
	}

	inline float Vec2::CrossProduct(const Vec2& a) const
	{
		return (x * a.y - y * a.x);
	}

	inline Vec2 Vec2::UnitVector() const
	{
		return operator/(GetLength());
	}

	inline void Vec2::Negate()
	{
		*this = operator*(-1);
	}

	inline Vec2 IVec2::GetNormal() const
	{
		float len = GetLength();
		if (len != 0.0f)
		{
			return Vec2(x / len, y / len);
		}

		return Vec2();
	}

	inline void Vec2::Normalize()
	{
		*this = GetNormal();
	}

	inline Vec2 Vec2::GetOrtho() const
	{
		return Vec2(-y, x);
	}

	inline void Vec2::Ortho()
	{
		*this = GetOrtho();
	}

	inline void Vec2::Print()
	{
		printf("(%.2f, %.2f)\n", x, y);
	}

	inline std::string Vec2::ToString()
	{
		return std::to_string(x) + " " + std::to_string(y);
	}

#pragma endregion

#pragma region Int Vector 2
	// ============== IVec2 Class ==============

	inline IVec2 IVec2::operator+(const IVec2& a) const
	{
		return IVec2(this->x + a.x, this->y + a.y);
	}
	inline IVec2 IVec2::operator-(const IVec2& a) const
	{
		return IVec2(this->x - a.x, this->y - a.y);
	}

	inline IVec2 IVec2::operator-(void) const
	{
		return operator*(-1);
	}

	inline IVec2 IVec2::operator*(const IVec2& a)const
	{
		return IVec2(this->x * a.x, this->y * a.y);
	}

	inline Vec2 IVec2::operator*(const float& a)const
	{
		return Vec2(this->x * a, this->y * a);
	}

	inline IVec2 IVec2::operator/(const float& a)const
	{
		if (a == 0.0f)
			return IVec2();
		return operator*(1 / a);
	}

	inline bool IVec2::operator==(const IVec2& b)const
	{
		return (x == b.x && y == b.y);
	}

	inline bool IVec2::operator!=(const IVec2& b) const
	{
		return (x != b.x || y != b.y);
	}

	inline int& IVec2::operator[](const size_t a)
	{
		return *((&x) + a);
	}

	inline float IVec2::LengthSquared() const
	{
		return (float)(x * x + y * y);
	}

	inline float IVec2::GetDistanceFromPoint(IVec2 a)
	{
		float dx = (float)(x - a.x);
		float dy = (float)(y - a.y);
		return std::sqrt(dx * dx + dy * dy);
	}

	inline float IVec2::GetLength() const
	{
		return sqrtf(LengthSquared());
	}

	inline bool IVec2::IsCollinearWith(IVec2 a)
	{
		return std::abs(x * a.y - y * a.x) < std::numeric_limits<float>::epsilon();
	}

	inline float IVec2::DotProduct(IVec2 a)
	{
		return (float)(a.x * x + a.y * y);
	}

	inline float IVec2::CrossProduct(IVec2 a)
	{
		return (float)(x * a.y - y * a.x);
	}

	inline IVec2 IVec2::UnitVector()
	{
		return operator/(GetLength());
	}

	inline void IVec2::Negate()
	{
		*this = operator*(-1);
	}

	inline IVec2 Vec2::GetNormal() const
	{
		float len = GetLength();
		if (len != 0.0f)
		{
			return IVec2(x / len, y / len);
		}

		return IVec2();
	}

	inline void IVec2::Normalize()
	{
		*this = GetNormal();
	}

	inline IVec2 IVec2::GetOrtho()
	{
		return IVec2(-y, x);
	}

	inline void IVec2::Ortho()
	{
		*this = GetOrtho();
	}

	inline void IVec2::Print()
	{
		printf("(%d, %d)\n", x, y);
	}

	inline std::string IVec2::ToString()
	{
		return std::to_string(x) + " " + std::to_string(y);
	}

#pragma endregion

#pragma region Vector 3
	// ============== Vec3 Class ==============

	inline Vec3::Vec3(const Vec4& a)
	{
		x = a.x; y = a.y; z = a.z;
	}

	inline Vec3 Vec3::operator=(const Vec4& b) const
	{
		return Vec3(b.x, b.y, b.z);
	}

	inline Vec3 Vec3::operator=(const IVec3& b) const
	{
		return Vec3((float)b.x, (float)b.y, (float)b.z);
	}

	inline Vec3 Vec3::operator=(const float b[3]) const
	{
		return Vec3(b[0], b[1], b[2]);
	}

	inline Vec3 Vec3::operator+(const Vec3& a) const
	{
		return Vec3(a.x + this->x, a.y + this->y, a.z + this->z);
	}

	inline void Vec3::operator+=(const Vec3& a)
	{
		*this = operator+(a);
	}

	inline void Vec3::operator-=(const Vec3& a)
	{
		*this = operator-(a);
	}

	inline Vec3 Vec3::operator-(const Vec3& a) const
	{
		return Vec3(this->x - a.x, this->y - a.y, this->z - a.z);
	}

	inline Vec3 Vec3::operator-(void) const
	{
		return operator*(-1);
	}

	inline Vec3 Vec3::operator*(const Vec3& a)const
	{
		return Vec3(this->x * a.x, this->y * a.y, this->z * a.z);
	}

	inline Vec3 Vec3::operator*(const IVec3& a) const
	{
		return Vec3(x * a.x, y * a.y, z * a.z);
	}

	inline Vec3 Vec3::operator*(const float& a)const
	{
		return Vec3(this->x * a, this->y * a, this->z * a);
	}

	inline void Vec3::operator*=(const float& a)
	{
		*this = operator*(a);
	}

	inline Vec3 Vec3::operator/(const float& a)const
	{
		if (a == 0.0)
			return Vec3();
		return operator*(1 / a);
	}

	inline Vec3 Vec3::operator/(const Vec3& other) const
	{
		return Vec3(x / other.x, y / other.y, z / other.z);
	}

	inline void Vec3::operator/=(const float& b)
	{
		*this = operator/(b);
	}

	inline bool Vec3::operator==(const Vec3& b)const
	{
		return (x == b.x && y == b.y && z == b.z);
	}

	inline bool Vec3::operator>(const Vec3& b) const
	{
		return (x > b.x || y > b.y || z > b.z);
	}

	inline bool Vec3::operator<(const Vec3& b) const
	{
		return (x < b.x || y < b.y || z < b.z);
	}

	inline bool Vec3::operator!=(const Vec3& b) const
	{
		return (x != b.x || y != b.y || z != b.z);
	}

	inline const float& Vec3::operator[](const size_t& a) const
	{
		return *((&x) + a);
	}

	inline float& Vec3::operator[](const size_t a)
	{
		return *((&x) + a);
	}

	inline float Vec3::LengthSquared() const
	{
		return (x * x + y * y + z * z);
	}

	inline float Vec3::GetLength() const
	{
		return sqrtf(LengthSquared());
	}

	inline bool Vec3::IsCollinearWith(const Vec3& a) const
	{
		return (DotProduct(a) < std::numeric_limits<float>::epsilon());
	}

	inline float Vec3::GetDistanceBetween(const Vec3& a) const
	{
		float i = a.x - x;
		float j = a.y - y;
		float h = a.z - z;
		return sqrtf(i * i + j * j + h * h);
	}

	inline float Vec3::DotProduct(const Vec3& a) const
	{
		return (a.x * x + a.y * y + a.z * z);
	}

	inline Vec3 Vec3::CrossProduct(const Vec3& a)
	{
		return { (y * a.z) - (z * a.y), (z * a.x) - (x * a.z), (x * a.y) - (y * a.x) };
	}

	inline Vec3 Vec3::UnitVector() const
	{
		return operator/(GetLength());
	}

	inline void Vec3::Negate()
	{
		*this = operator*(-1);
	}

	inline Vec3 Vec3::ToRadians() const
	{
		return Vec3(x * DegToRad, y * DegToRad, z * DegToRad);
	}

	inline Vec3 Vec3::ToDegrees() const
	{
		return Vec3(x * RadToDeg, y * RadToDeg, z * RadToDeg);
	}

	inline Vec3 Vec3::Clamp(const Vec3& min, const Vec3& max) const
	{
		Vec3 tmp = *this;
		if (tmp < min)
			tmp = min;
		else if (tmp > max)
			tmp = max;
		return tmp;
	}

	inline Vec3 Vec3::ClampMagnitude(const Vec3& vector, float maxLength)
	{
		float sqrmag = vector.LengthSquared();
		if (sqrmag > maxLength * maxLength)
		{
			float mag = (float)sqrtf(sqrmag);
			float normalized_x = vector.x / mag;
			float normalized_y = vector.y / mag;
			float normalized_z = vector.z / mag;
			return Vec3(normalized_x * maxLength,
				normalized_y * maxLength,
				normalized_z * maxLength);
		}
		return vector;
	}

	inline Vec3 Vec3::GetNormal() const
	{
		float len = std::sqrt(x * x + y * y + z * z);

		if (len != 0.)
		{

			return Vec3(x / len, y / len, z / len);
		}
		return Vec3(x, y, z);
	}

	inline void Vec3::Normalize()
	{
		*this = GetNormal();
	}

	inline Quat Vec3::ToQuaternion() const
	{
		auto a = this->ToRadians();
		// Calculate the cosine of each half angle
		float cy = std::cosf(a.z * 0.5f);
		float cp = std::cosf(a.y * 0.5f);
		float cr = std::cosf(a.x * 0.5f);

		// Calculate the sine of each half angle
		float sy = std::sinf(a.z * 0.5f);
		float sp = std::sinf(a.y * 0.5f);
		float sr = std::sinf(a.x * 0.5f);

		return { cy * cp * sr - sy * sp * cr , sy * cp * sr + cy * sp * cr, sy * cp * cr - cy * sp * sr, cy * cp * cr + sy * sp * sr };
	}

	inline Vec3 Vec3::Lerp(const Vec3& a, const Vec3& b, float t)
	{
		if (t < 0)
			return a;
		else if (t >= 1)
			return b;
		return a * (1 - t) + b * t;
	}


	inline std::string Vec3::ToString()
	{
		return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z);
	}

	inline void Vec3::Print() const
	{
		printf("(%.2f, %.2f, %.2f)\n", x, y, z);
	}
#pragma endregion

#pragma region Int Vector 3
	// ============== IVec3 Class ==============

	inline IVec3 IVec3::operator=(const Vec3& b)
	{
		return IVec3((int)b.x, (int)b.y, (int)b.z);
	}

	inline IVec3 IVec3::operator=(const int b[3])
	{
		return IVec3(b[0], b[1], b[2]);
	}

	inline IVec3 IVec3::operator+(const IVec3& a) const
	{
		return IVec3(x + a.x, y + a.y, z + a.z);
	}

	inline IVec3 IVec3::operator-(void) const
	{
		return operator*(-1);
	}

	inline IVec3 IVec3::operator-(const IVec3& a) const
	{
		return IVec3(x - a.x, y - a.y, z - a.z);
	}

	inline void IVec3::operator+=(const IVec3& a)
	{
		*this = operator+(a);
	}

	inline void IVec3::operator-=(const IVec3& a)
	{
		*this = operator-(a);
	}

	inline IVec3 IVec3::operator*(const IVec3& a) const
	{
		return IVec3(x * a.x, y * a.y, z * a.z);
	}

	inline IVec3 IVec3::operator*(const Vec3& a) const
	{
		return IVec3(x * (int)a.x, y * (int)a.y, z * (int)a.z);
	}

	inline IVec3 IVec3::operator*(const float& a) const
	{
		return IVec3(x * (int)a, y * (int)a, z * (int)a);
	}

	inline void IVec3::operator*=(const float& a)
	{
		*this = operator*(a);
	}

	inline IVec3 IVec3::operator/(const float& b) const
	{
		return IVec3(x / (int)b, y / (int)b, z / (int)b);
	}

	inline IVec3 IVec3::operator/(const Vec3& other) const
	{
		return IVec3(x / (int)other.x, y / (int)other.y, z / (int)other.z);
	}

	inline void IVec3::operator/=(const float& b)
	{
		*this = operator/(b);
	}

	inline bool IVec3::operator==(const IVec3& b) const
	{
		return (b.x == x && b.y == y && b.z == z);
	}

	inline bool IVec3::operator>(const IVec3& b) const
	{
		return (x > b.x || y > b.y || z > b.z);
	}

	inline bool IVec3::operator<(const IVec3& b) const
	{
		return (x < b.x || y < b.y || z < b.z);
	}

	inline bool IVec3::operator!=(const IVec3& b) const
	{
		return (b.x != x || b.y != y || b.z != z);
	}

	inline int& IVec3::operator[](const size_t& a)
	{
		return *((&x) + a);
	}

	inline std::string IVec3::ToString()
	{
		return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z);
	}

	inline void IVec3::Print() const
	{
		printf("(%d, %d, %d)\n", x, y, z);
	}
#pragma endregion

#pragma region Vector 4
	Vec4 Math::Vec4::operator+(const Vec4& a) const
	{
		return { a.x + x, a.y + y, a.z + z, a.w + w };
	}

	Vec4 Math::Vec4::operator-(void) const
	{
		return operator*(-1);
	}

	Vec4 Math::Vec4::operator-(const Vec4& a) const
	{
		return { x - a.x, y - a.y, z - a.z, w - a.w };
	}

	Vec4 Math::Vec4::operator*(const Vec4& a) const
	{
		return { x * a.x, y * a.y, z * a.z, w * a.w };
	}

	Vec4 Math::Vec4::operator*(const float& a) const
	{
		return { x * a, y * a, z * a, w * a };
	}

	Vec4 Math::Vec4::operator/(const float& b) const
	{
		return { x / b, y / b, z / b, w / b };
	}

	inline bool Vec4::operator==(const Vec4& b) const
	{
		return (x == b.x && y == b.y && z == b.z && w == b.w);
	}

	inline const float& Vec4::operator[](const size_t a) const
	{
		return *((&x) + a);
	}

	inline float& Vec4::operator[](const size_t a)
	{
		return *((&x) + a);
	}

	inline bool Vec4::IsCollinearWith(const Vec4& a) const
	{
		return (DotProduct(a) < std::numeric_limits<float>::epsilon());
	}

	inline float Vec4::DotProduct(const Vec4& a) const
	{
		return (a.x * x + a.y * y + a.z * z);
	}

	inline Vec4 Vec4::CrossProduct(const Vec4& a) const
	{
		return Vec4((y * a.z) - (z * a.y), (z * a.x) - (x * a.z), (x * a.y) - (y * a.x), 1.0f);
	}

	inline Vec4 Vec4::UnitVector() const
	{
		Vec4 homogenized = GetHomogenize();
		return homogenized / homogenized.GetVector().GetLength();
	}

	inline Vec3 Vec4::GetVector() const
	{
		return Vec3(x, y, z);
	}

	inline float Vec4::LengthSquared() const
	{
		return GetHomogenize().GetVector().LengthSquared();
	}

	inline float Vec4::GetLength() const
	{
		return sqrtf(LengthSquared());
	}

	inline Vec4 Vec4::GetHomogenize() const
	{
		return Vec4(GetVector() / w);
	}

	inline void Vec4::Homogenize()
	{
		*this = GetHomogenize();
	}

	inline void Vec4::Print() const
	{
		printf("(%.2f, %.2f, %.2f, %.2f)\n", x, y, z, w);
	}

	inline std::string Vec4::ToString() const
	{
		return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + " " + std::to_string(w);
	}

#pragma  endregion

#pragma  region Mat4

	inline Mat4::Mat4(float diagonal)
	{
		content[0][0] = 1;
		content[1][1] = 1;
		content[2][2] = 1;
		content[3][3] = 1;
	}

	inline Mat4::Mat4(const float* data)
	{
		std::memcpy(this->content, data, sizeof(this->content));
	}

	inline Mat4::Mat4(const double* data)
	{
		std::memcpy(this->content, data, sizeof(this->content));
	}

	inline Mat4 Mat4::operator*(const Mat4& a) const
	{
		Mat4 out;
		for (size_t j = 0; j < 4; j++)
		{
			for (size_t i = 0; i < 4; i++)
			{
				float res = 0;
				for (size_t k = 0; k < 4; k++)
					res += content[j][k] * a.content[k][i];

				out.content[j][i] = res;
			}
		}
		return out;
	}

	inline Vec4 Mat4::operator*(const Vec4& a) const
	{
		Vec4 out;
		for (size_t i = 0; i < 4; i++)
		{
			float res = 0;
			for (size_t k = 0; k < 4; k++) res += content[i][k] * a[k];
			out[i] = res;
		}
		return out;
	}

	inline Mat4 Mat4::operator+(const Mat4& a) const
	{
		Mat4 tmp;
		for (size_t j = 0; j < 4; j++)
		{
			for (size_t i = 0; i < 4; i++)
			{
				tmp.content[j][i] = content[j][i] + a.content[j][i];
			}
		}
		return tmp;
	}

	inline float& Mat4::operator[](const size_t a)
	{
		return content[a / 4][a % 4];
	}

	inline float& Mat4::at(const unsigned char x, const unsigned char y)
	{
		if (x > 3 || y > 3)
			return content[0][0];
		return content[y][x];
	}

	inline Mat4 Mat4::CreateTransformMatrix(const Vec3& position, const Vec3& rotation, const Vec3& scale)
	{
		return CreateTranslationMatrix(position) * CreateYRotationMatrix(rotation.y) * CreateXRotationMatrix(rotation.x) * CreateZRotationMatrix(rotation.z) * CreateScaleMatrix(scale);
	}

	inline Mat4 Mat4::CreateTransformMatrix(const Vec3& position, const Quat& rotation, const Vec3& scale)
	{
		// TODO
		return Mat4();
	}

	inline Mat4 Mat4::CreateTranslationMatrix(const Vec3& translation)
	{
		Mat4 out(1);
		for (size_t i = 0; i < 3; i++) out.at(3, (const unsigned char)i) = translation[i];
		return out;
	}

	inline Mat4 Mat4::CreateScaleMatrix(const Vec3& scale)
	{
		Mat4 out(1);
		for (size_t i = 0; i < 3; i++) out.at((const unsigned char)i, (const unsigned char)i) = scale[i];
		out.content[3][3] = 1;
		return out;
	}

	inline Mat4 Mat4::CreateXRotationMatrix(float angle)
	{
		Mat4 out(1);
		float radA = ToRadians(angle);
		float cosA = cosf(radA);
		float sinA = sinf(radA);
		out.at(1, 1) = cosA;
		out.at(2, 1) = -sinA;
		out.at(1, 2) = sinA;
		out.at(2, 2) = cosA;
		return out;
	}

	inline Mat4 Mat4::CreateYRotationMatrix(float angle)
	{
		Mat4 out(1);
		float radA = ToRadians(angle);
		float cosA = cosf(radA);
		float sinA = sinf(radA);
		out.at(0, 0) = cosA;
		out.at(2, 0) = sinA;
		out.at(0, 2) = -sinA;
		out.at(2, 2) = cosA;
		return out;
	}

	inline Mat4 Mat4::CreateZRotationMatrix(float angle)
	{
		Mat4 out(1);
		float radA = ToRadians(angle);
		float cosA = cosf(radA);
		float sinA = sinf(radA);
		out.at(0, 0) = cosA;
		out.at(1, 0) = -sinA;
		out.at(0, 1) = sinA;
		out.at(1, 1) = cosA;
		return out;
	}

	inline Mat4 Mat4::CreateRotationMatrix(const Vec3& rotation)
	{
		return CreateYRotationMatrix(rotation.y) * CreateXRotationMatrix(rotation.x) * CreateZRotationMatrix(rotation.z);
	}

	inline Mat4 Mat4::CreatePerspectiveProjectionMatrix(float Near, float Far, float fov)
	{
		float s = 1.0f / tanf(ToRadians(fov / 2.0f));
		float param1 = -Far / (Far - Near);
		float param2 = param1 * Near;
		Mat4 out;
		out.at(0, 0) = s;
		out.at(1, 1) = s;
		out.at(2, 2) = param1;
		out.at(3, 2) = -1;
		out.at(2, 3) = param2;
		return out;
	}

	inline Quat Mat4::ToQuaternion()
	{
		float w = sqrtf(1 + at(0, 0) + at(1, 1) + at(2, 2)) / 2;
		return Quat((at(2, 1) - at(1, 2)) / (4 * w), (at(0, 2) - at(2, 0)) / (4 * w), (at(1, 0) - at(0, 1)) / (4 * w), w);
	}

	inline Vec3 Mat4::GetPosition() const
	{
		return Vec3(content[0][3], content[1][3], content[2][3]);
	}

	inline Quat Mat4::GetRotation()
	{
		Vec3 sca = GetScale();
		Mat4 rotMat;
		// Get Rotation Matrix.
		if (sca == Vec3(0))
			sca = Vec3(0.0001f);
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

		return rotMat.ToQuaternion();
	}

	inline Vec3 Mat4::GetEulerRotation()
	{
		Vec3 sca = GetScale();
		Mat4 rotMat;
		// Get Rotation Matrix.
		if (sca == Vec3(0))
			sca = Vec3(0.0001f);
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
		return Vec3(thetaX, thetaY, thetaZ).ToDegrees();
	}

	inline Vec3 Mat4::GetScale() const
	{
		// World Scale equal length of columns of the model matrix.
		float x = Vec3(content[0][0], content[0][1], content[0][2]).GetLength();
		float y = Vec3(content[1][0], content[1][1], content[1][2]).GetLength();
		float z = Vec3(content[2][0], content[2][1], content[2][2]).GetLength();
		return Vec3(x, y, z);
	}

	inline Mat4 Mat4::CreateInverseMatrix()
	{
		// Find determinant of matrix
		Mat4 inverse(1);
		float det = GetDeterminant(4);
		if (det == 0.0f)
		{
			printf("ERROR");
			return Mat4::Identity();
		}

		// Find adjoint
		Mat4 adj = CreateAdjMatrix();

		// Find Inverse using formula "inverse(A) = adj(A)/det(A)"
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				inverse.content[i][j] = adj.content[i][j] / float(det);

		return inverse;
	}

	inline Mat4 Mat4::CreateAdjMatrix()
	{
		// temp is used to store cofactors of matrix
		Mat4 temp;
		Mat4 adj;
		int sign = 1;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				// Get cofactor of matrix[i][j]
				temp = GetCofactor(i, j, 4);

				// sign of adj positive if sum of row
				// and column indexes is even.
				sign = ((i + j) % 2 == 0) ? 1 : -1;

				// Interchanging rows and columns to get the
				// transpose of the cofactor matrix
				adj.content[j][i] = (float)((sign) * (temp.GetDeterminant(3)));
			}
		}
		return adj;
	}

	inline Mat4 Mat4::GetCofactor(int p, int q, int n)
	{
		Mat4 mat;
		int i = 0, j = 0;
		// Looping for each element of the matrix
		for (int row = 0; row < n; row++)
		{
			for (int col = 0; col < n; col++)
			{
				//  Copying into temporary matrix only those element
				//  which are not in given row and column
				if (row != p && col != q)
				{
					mat.content[i][j++] = content[row][col];

					// Row is filled, so increase row index and
					// reset col index
					if (j == n - 1)
					{
						j = 0;
						i++;
					}
				}
			}
		}
		return mat;
	}

	inline float Mat4::GetDeterminant(int n) const
	{
		if (n == 2)
		{
			float x = content[0][0] * content[1][1] - content[1][0] * content[0][1];
			return content[0][0] * content[1][1] - content[1][0] * content[0][1];
		}
		else if (n == 3)
		{
			float result = content[0][0] * content[1][1] * content[2][2]
				- content[0][0] * content[2][1] * content[1][2]
				+ content[1][0] * content[2][1] * content[0][2]
				- content[1][0] * content[0][1] * content[2][2]
				+ content[2][0] * content[0][1] * content[1][2]
				- content[2][0] * content[1][1] * content[0][2];
			return result;
		}
		else if (n == 4)
		{
			float result = content[0][0] * (content[1][1] * content[2][2] * content[3][3] // a(fkp
				- content[1][1] * content[3][2] * content[2][3] //flo
				- content[2][1] * content[1][2] * content[3][3] //gjp
				+ content[2][1] * content[3][2] * content[1][3] //gln
				+ content[3][1] * content[1][2] * content[2][3] //hjo
				- content[3][1] * content[2][2] * content[1][3]) // hkn

				- content[1][0] * (content[0][1] * content[2][2] * content[3][3] //b(ekp
					- content[0][1] * content[3][2] * content[2][3] // elo
					- content[2][1] * content[0][2] * content[3][3] //gip
					+ content[2][1] * content[3][2] * content[0][3] //glm
					+ content[3][1] * content[0][2] * content[2][3] //hio
					- content[3][1] * content[2][2] * content[0][3]) //hkm

				+ content[2][0] * (content[0][1] * content[1][2] * content[3][3] // c(ejp
					- content[0][1] * content[3][2] * content[1][3] //eln
					- content[1][1] * content[0][2] * content[3][3] //fip
					+ content[1][1] * content[3][2] * content[0][3] //flm
					+ content[3][1] * content[0][2] * content[1][3] //hin
					- content[3][1] * content[1][2] * content[0][3]) //hjm

				- content[3][0] * (content[0][1] * content[1][2] * content[2][3] // d(ejo
					- content[0][1] * content[2][2] * content[1][3] //ekn
					- content[1][1] * content[0][2] * content[2][3] //fio
					+ content[1][1] * content[2][2] * content[0][3] //fkm
					+ content[2][1] * content[0][2] * content[1][3] //gin
					- content[2][1] * content[1][2] * content[0][3]); //gjm
			return result;
		}
		else return 0.0f;
	}

	inline Mat4 Mat4::TransposeMatrix() const
	{
		return Mat4();
	}

	inline void Mat4::Print() const
	{
		for (int j = 0; j < 4; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				if (content[j][i] >= 0.0f) printf(" ");
				printf("%.2f", content[j][i]);
			}
			printf("\n");
		}
		printf("\n");
	}

	inline std::string Math::Mat4::ToString() const
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

#pragma  endregion
#pragma region Quaternion
	inline Quat Quat::operator+(const Quat& a) const
	{
		return Quat(x + a.x, y + a.y, z + a.z, w + a.w);
	}

	inline Quat Quat::operator-(const Quat& a) const
	{
		return Quat(x - a.x, y - a.y, z - a.z, w - a.w);
	}

	inline Quat Quat::operator*(const Quat& a) const
	{
		return Quat(
			w * a.x + x * a.w + y * a.z - z * a.y,
			w * a.y + y * a.w + z * a.x - x * a.z,
			w * a.z + z * a.w + x * a.y - y * a.x,
			w * a.w - x * a.x - y * a.y - z * a.z);
	}

	inline Quat Quat::operator*(const float& a) const
	{
		return Quat(this->x * a, this->y * a, this->z * a, this->w * a);
	}

	inline Vec3 Quat::operator*(const Vec3& a) const
	{
		Vec3 vector;
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
		return { (1.f - (yy + zz)) * a.x + (xy - wz) * a.y + (xz + wy) * a.z ,
				(xy + wz) * a.x + (1.f - (xx + zz)) * a.y + (yz - wx) * a.z ,
				(xz - wy) * a.x + (yz + wx) * a.y + (1.f - (xx + yy)) * a.z };
	}

	inline float& Quat::operator[](const size_t index)
	{
		return *((&x) + index);
	}

	inline Quat Quat::AngleAxis(float angle, Vec3 axis)
	{
		Quat q;
		float rad = angle * DegToRad;
		axis.Normalize();
		q.w = cosf(rad / 2);
		q.x = sinf(rad / 2) * axis.x;
		q.y = sinf(rad / 2) * axis.y;
		q.z = sinf(rad / 2) * axis.z;
		return { sinf(rad / 2) * axis.x, sinf(rad / 2) * axis.y, sinf(rad / 2) * axis.z, cosf(rad / 2) };
	}
	inline Quat Quat::LookRotation(Vec3 forward, Vec3 up)
	{
		forward.Normalize();
		Vec3 vector = forward.GetNormal();
		Vec3 vector2 = up.CrossProduct(vector).GetNormal();
		Vec3 vector3 = vector.CrossProduct(vector2);
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
		Quat quaternion;
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
		return { (m20 + m02) * num2 , (m21 + m12) * num2, 0.5f * num5,(m01 - m10) * num2 };
	}

	inline Quat Quat::SLerp(const Quat& a, const Quat& b, float time)
	{
		if (time < 0.0f)
			return a;
		else if (time >= 1.0f)
			return b;
		float d = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
		float s0, s1;
		float sd = (float)((d > 0.0f) - (d < 0.0f));

		d = fabs(d);

		if (d < 0.9995f)
		{
			float s = sqrtf(1.0f - d * d);
			float a = atan2f(s, d);
			float c = cosf(time * a);


			s1 = sqrtf(1.0f - c * c) / s;
			s0 = c - d * s1;
		}
		else
		{
			s0 = 1.0f - time;
			s1 = time;
		}

		return a * s0 + b * sd * s1;
	}

	inline void Quat::Inverse()
	{
		*this = GetInverse();
	}
	inline Quat Quat::GetInverse() const
	{
		float d = w * w + x * x + y * y + z * z;
		if (x == 0 && y == 0 && z == 0 && w == 0)
			return *this;
		else
			return Quat(-x / d, -y / d, -z / d, w / d);
	}
	inline void Quat::Normalize()
	{
		*this = GetNormal();
	}
	inline Quat Quat::GetNormal() const
	{
		float mag = sqrtf(Dot(*this));

		if (mag < FLT_MIN)
			return Quat::Identity();
		else
			return Quat(x / mag, y / mag, z / mag, w / mag);
	}

	inline void Quat::Conjugate()
	{
		*this = GetConjugate();
	}

	inline Quat Quat::GetConjugate() const
	{
		return Quat(-x, -y, -z, w);
	}

	inline float Quat::Dot(const Quat& a) const
	{
		return x * a.x + y * a.y + z * a.z + w * a.w;
	}
	inline Vec3 Quat::ToEuler() const
	{
		// IT WORKS
		Vec3 euler;
		Mat4 rotMat = ToRotationMatrix();

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
		return Vec3(thetaX, thetaY, thetaZ).ToDegrees();
	}

	inline Mat4 Quat::ToRotationMatrix() const
	{
		Mat4 m = Mat4::Identity();
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

	inline void Quat::Print() const
	{
		printf("Quaternion { %f, %f, %f, %f}\n", x, y, z, w);
	}
	inline std::string Quat::ToString() const
	{
		return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + " " + std::to_string(w);
	}
#pragma endregion
}