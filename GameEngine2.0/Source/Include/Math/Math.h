#pragma once
#include <iostream>

#define PI 3.14159265358979323846264f
#define DegToRad 1/180.f * PI
#define RadToDeg 180.f / PI

namespace Math
{
	class IVec2;
	class IVec3;
	class IVec4;
	class Vec3;
	class Vec4;
	class Quat;

	inline float ToRadians(float in);

	inline float ToDegrees(float in);

	// Return the number cut between the two float values.
	inline float Cut(float in, float min, float max);

	// Return the number cut between the two int values.
	inline int CutInt(int in, int min, int max);

	// Return the number cut between the two int values.
	inline bool EquivalentFloat(float a, float b, float dif);

	// Function Modulo.
	inline float Mod(float in, float value);

	// Return the Minimum number of the two given.
	inline float Min(float a, float b);

	// Return the Maximum number of the two given.
	inline float Max(float a, float b);

	inline Vec3 GetSphericalCoords(float r, float t, float p);

	class Vec2
	{
	public:
		float x = 0.f, y = 0.f;

		// Return a new empty Vec2
		Vec2() : x(0), y(0) {}

		// Return a new Vec2 initialized with 'a' and 'b'
		Vec2(float a, float b) : x(a), y(b) {}

		// Return a new Vec2 initialized with 'in'
		Vec2(const Vec2& in) : x(in.x), y(in.y) {}

		inline Vec2(const IVec2& in);

		// Return a new vector which is the sum of 'a' and 'b'
		inline Vec2 operator+(const Vec2& a) const;

		// Return a new vector which is the subtraction of 'a' and 'b'
		inline Vec2 operator-(const Vec2& a) const;

		// Return a new vector which is the negative
		inline Vec2 operator-(void) const;

		// Return the result of the arithmetic multiplication of 'a' and 'b'
		inline Vec2 operator*(const Vec2& a) const;

		// Return the result of the arithmetic multiplication of 'a' and 'b'
		inline Vec2 operator*(const float& a) const;

		// Return the result of the arithmetic division of 'a' and 'b'
		inline Vec2 operator/(const float& b) const;

		inline bool operator==(const Vec2& b) const;

		inline bool operator!=(const Vec2& b) const;

		inline float& operator[](const size_t a);

		// Return a vector of length 'in' and with an opposite direction
		inline void Negate();

		// Return the normal vector of 'in'
		inline IVec2 GetNormal() const;

		// normalize the vector
		inline void Normalize();

		// Get The orthonormal of the Vector.
		inline Vec2 GetOrtho() const;

		// Set The Vector to the orthonormal.
		inline void Ortho();

		// Return the length squared of 'in'
		inline float LengthSquared() const;

		//Return the distance between 'in' an 'a'
		inline float GetDistanceFromPoint(const Vec2& a) const;

		// Return the length of the given Vector
		inline float GetLength() const;

		// Return true if 'a' and 'b' are collinear (Precision defined by VEC2D_COLLINEAR_PRECISION)
		inline bool IsCollinearWith(const Vec2& a) const;

		// Return the dot product of 'a' and 'b'
		inline float DotProduct(const Vec2& a) const;

		// Return the z component of the cross product of 'a' and 'b'
		inline float CrossProduct(const Vec2& a) const;

		// Return a vector with the same direction that 'in', but with length 1
		inline Vec2 UnitVector() const;

		// Print the Vec2 with .2f
		inline void Print();

		// Return the Vec2 in string.
		inline std::string ToString();
	};

	class IVec2
	{
	public:
		int x = 0, y = 0;

		// Return a new empty IVec2
		IVec2() : x(0), y(0) {}

		// Return a new IVec2 initialized with 'a' and 'b'
		IVec2(int a, int b) : x(a), y(b) {}

		IVec2(float a, float b) : x((int)a), y((int)b) {}

		// Return a new IVec2 initialized with 'in'
		IVec2(const IVec2& in) : x(in.x), y(in.y) {}

		// Return a new Vec2 initialized with 'in'
		IVec2(const Vec2& in) : x((int)in.x), y((int)in.y) {}

		// Return a new vector which is the sum of 'a' and 'b'
		inline IVec2 operator+(const IVec2& a) const;

		// Return a new vector which is the subtraction of 'a' and 'b'
		inline IVec2 operator-(const IVec2& a) const;

		// Return a new vector which is the negative
		inline IVec2 operator-(void) const;

		// Return the result of the arithmetic multiplication of 'a' and 'b'
		inline IVec2 operator*(const IVec2& a) const;

		// Return the result of the arithmetic multiplication of 'a' and 'b'
		inline Vec2 operator*(const float& a) const;

		// Return the result of the arithmetic division of 'a' and 'b'
		inline IVec2 operator/(const float& b) const;

		inline bool operator==(const IVec2& b) const;

		inline bool operator!=(const IVec2& b) const;

		inline int& operator[](const size_t a);

		// Return a vector of length 'in' and with an opposite direction
		inline void Negate();

		// Return the normal vector of 'in'
		inline Vec2 GetNormal() const;

		// normalize the vector
		inline void Normalize();

		// Get The orthonormal of the Vector.
		inline IVec2 GetOrtho();

		// Set The Vector to the orthonormal.
		inline void Ortho();

		// Return the length squared of 'in'
		inline float LengthSquared() const;

		//Return the distance between 'in' an 'a'
		inline float GetDistanceFromPoint(IVec2 a);

		// Return the length of the given Vector
		inline float GetLength() const;

		// Return true if 'a' and 'b' are collinear (Precision defined by VEC2D_COLLINEAR_PRECISION)
		inline bool IsCollinearWith(IVec2 a);

		// Return the dot product of 'a' and 'b'
		inline float DotProduct(IVec2 a);

		// Return the z component of the cross product of 'a' and 'b'
		inline float CrossProduct(IVec2 a);

		// Return a vector with the same direction that 'in', but with length 1
		inline IVec2 UnitVector();

		// Print the Vec2 with .2f
		inline void Print();

		// Return the Vec2 in string.
		inline std::string ToString();
	};

	class Vec3
	{
	public:
		float x = 0, y = 0, z = 0;

		// Return a new empty Vector3
		Vec3() : x(0), y(0), z(0) {}

		// Return a new Vec3 initialized with 'a' and 'b'
		Vec3(float a, float b, float c) : x(a), y(b), z(c) {}

		Vec3(float content) : x(content), y(content), z(content) {}

		// Return a new Vec3 initialized with 'in'
		Vec3(const Vec3& in) : x(in.x), y(in.y), z(in.z) {}

		Vec3(const Vec2& a) : x(a.x), y(a.y), z(0) {}

		inline Vec3(const Vec4& a);

		inline Vec3 operator=(const Vec4& b) const;

		inline Vec3 operator=(const IVec3& b) const;

		inline Vec3 operator=(const float b[3]) const;

		// Return a new vector which is the sum of 'a' and 'b'
		inline Vec3 operator+(const Vec3& a) const;

		// Return the negate.
		inline Vec3 operator-(void) const;

		// Return a new vector which is the subtraction of 'a' and 'b'
		inline Vec3 operator-(const Vec3& a) const;

		inline void operator+=(const Vec3& a);

		inline void operator-=(const Vec3& a);

		// Return the result of the arithmetic multiplication of 'a' and 'b'
		inline Vec3 operator*(const Vec3& a) const;

		// Return the result of the arithmetic multiplication of 'a' and 'b'
		inline Vec3 operator*(const IVec3& a) const;

		// Return the result of the arithmetic multiplication of 'a' and 'b'
		inline Vec3 operator*(const float& a) const;

		// Return the result of the arithmetic multiplication of 'a' and 'b'
		inline void operator*=(const float& a);

		// \return the result of the arithmetic division of 'a' and 'b'
		// \param b : float value to divide
		inline Vec3 operator/(const float& b) const;

		inline Vec3 operator/(const Vec3& other) const;

		inline void operator/=(const float& b);

		inline bool operator==(const Vec3& b) const;

		inline bool operator>(const Vec3& b) const;

		inline bool operator<(const Vec3& b) const;

		inline bool operator!=(const Vec3& b) const;

		inline const float& operator[](const size_t& a) const;

		inline float& operator[](const size_t a);

		// Return the length squared of 'in'
		inline float LengthSquared() const;

		// Return the length of the given Vector
		inline float GetLength() const;

		// Return true if 'a' and 'b' are collinear (Precision defined by VEC_COLLINEAR_PRECISION)
		inline bool IsCollinearWith(const Vec3& a) const;

		inline float GetDistanceBetween(const Vec3& a) const;

		// Return the dot product of 'a' and 'b'
		inline float DotProduct(const Vec3& a) const;

		// Return the z component of the cross product of 'a' and 'b'
		inline Vec3 CrossProduct(const Vec3& a);

		// Return a vector with the same direction that 'in', but with length 1
		inline Vec3 UnitVector() const;

		// Return a vector of length 'in' and with an opposite direction
		inline void Negate();

		// Convert degree to rad
		inline Vec3 ToRadians() const;

		// Convert rad to Degree
		inline Vec3 ToDegrees() const;

		inline Vec3 Clamp(const Vec3& min, const Vec3& max) const;

		inline static Vec3 ClampMagnitude(const Vec3&, float);

		inline static Vec3 Forward() { return Vec3(0, 0, 1); }

		inline static Vec3 Right() { return Vec3(1, 0, 0); }

		inline static Vec3 Up() { return Vec3(0, 1, 0); }

		static Vec3 Lerp(const Math::Vec3& a, const Math::Vec3& b, float t);

		inline Vec3 GetNormal() const;

		inline void Normalize();

		inline Quat ToQuaternion() const;

		inline std::string ToString();

		// Print the Vec3
		inline void Print() const;
	};

	class IVec3
	{
	public:
		int x = 0, y = 0, z = 0;

		IVec3() : x(0), y(0), z(0) {}

		IVec3(int a, int b, int c) : x(a), y(b), z(c) {}

		IVec3(int content) : x(content), y(content), z(content) {}

		IVec3(Vec3 a) : x((int)a.x), y((int)a.y), z((int)a.z) {}

		inline IVec3 operator=(const Vec3& b);

		inline IVec3 operator=(const int b[3]);

		// Return a new vector which is the sum of 'a' and 'b'
		inline IVec3 operator+(const IVec3& a) const;

		// Return the negate.
		inline IVec3 operator-(void) const;

		// Return a new vector which is the subtraction of 'a' and 'b'
		inline IVec3 operator-(const IVec3& a) const;

		inline void operator+=(const IVec3& a);

		inline void operator-=(const IVec3& a);

		// Return the result of the arithmetic multiplication of 'a' and 'b'
		inline IVec3 operator*(const IVec3& a) const;

		// Return the result of the arithmetic multiplication of 'a' and 'b'
		inline IVec3 operator*(const Vec3& a) const;

		// Return the result of the arithmetic multiplication of 'a' and 'b'
		inline IVec3 operator*(const float& a) const;

		// Return the result of the arithmetic multiplication of 'a' and 'b'
		inline void operator*=(const float& a);

		// \return the result of the arithmetic division of 'a' and 'b'
		// \param b : float value to divide
		inline IVec3 operator/(const float& b) const;

		inline IVec3 operator/(const Vec3& other) const;

		inline void operator/=(const float& b);

		inline bool operator==(const IVec3& b) const;

		inline bool operator>(const IVec3& b) const;

		inline bool operator<(const IVec3& b) const;

		inline bool operator!=(const IVec3& b) const;

		inline int& operator[](const size_t& a);

		inline std::string ToString();

		// Print the Vec3
		inline void Print() const;
	};

	class Vec4
	{
	public:
		float x = 0, y = 0, z = 0, w = 0;

		// Return a new empty Vec4
		Vec4() : x(0), y(0), z(0), w(0) {}

		Vec4(float a) : x(a), y(a), z(a), w(a) {}

		// Return a new Vec4 initialized with 'a', 'b', 'c' and 'd'
		Vec4(float a, float b, float c, float d = 1) : x(a), y(b), z(c), w(d) {}

		// Return a new Vec4 initialized with 'in'
		Vec4(const Vec3& in, float wIn = 1.0f) : x(in.x), y(in.y), z(in.z), w(wIn) {}

		// Return a new Vec4 initialized with 'in'
		Vec4(const Vec4& in) : x(in.x), y(in.y), z(in.z), w(in.w) {}

		inline Vec4 operator+(const Vec4& a) const;

		inline Vec4 operator-(void) const;

		inline Vec4 operator-(const Vec4& a) const;

		inline Vec4 operator*(const Vec4& a) const;

		inline Vec4 operator*(const float& a) const;

		inline Vec4 operator/(const float& b) const;

		inline bool operator==(const Vec4& b) const;

		inline const float& operator[](const size_t a) const;

		inline float& operator[](const size_t a);
		/**
		@brief Check if the current Vec4 is collinear with a given Vec4
		@param a The Vec4 to check for collinearity with
		@return true if the current Vec4 is collinear with the given Vec4, false otherwise
		*/
		inline bool IsCollinearWith(const Vec4& a) const;
		/**
		@brief Compute the dot product of the current Vec4 with another given Vec4
		@param a The Vec4 to compute the dot product with
		@return The dot product of the current Vec4 with the given Vec4
		*/
		inline float DotProduct(const Vec4& a) const;
		/**
		@brief Compute the cross product of the current Vec4 with another given Vec4
		@param a The Vec4 to compute the cross product with
		@return The cross product of the current Vec4 with the given Vec4
		*/
		inline Vec4 CrossProduct(const Vec4& a) const;
		/**
		@brief Return a vector with the same direction that 'in', but with length 1
		@return The unit vector of the current Vec4
		*/
		inline Vec4 UnitVector() const;
		/**
		@brief Get a Vec3 representation of the current Vec4, with the last component dropped
		@return The Vec3 representation of the current Vec4
		*/
		inline Vec3 GetVector() const;
		/**
		@brief Compute the square of the length of the current Vec4
		@return The square of the length of the current Vec4
		*/
		inline float LengthSquared() const;
		/**
		@brief Compute the length of the current Vec4
		@return The length of the current Vec4
		*/
		inline float GetLength() const;
		/**
		@brief Get the homogenized form of the current Vec4
		@return The homogenized form of the current Vec4
		*/
		inline Vec4 GetHomogenize() const;
		/**
		@brief Homogenize the current Vec4
		*/
		inline void Homogenize();
		/**
		@brief Print the current Vec4
		*/
		inline void Print() const;
		/**
		@brief Convert the Vec4 into a string
		@return the Vec4 converted to a string
		*/
		inline std::string ToString() const;
	};

	class Mat4
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

		Mat4() {}

		inline Mat4(float diagonal);

		inline Mat4(const float* data);

		inline Mat4(const double* data);

		inline Mat4 operator*(const Mat4& a) const;

		inline Vec4 operator*(const Vec4& a) const;

		inline Mat4 operator+(const Mat4& a) const;

		inline float& operator[](const size_t a);

		inline float& at(const unsigned char x, const unsigned char y);

		static Mat4 Identity() { return { 1.f }; }

		static inline Mat4 CreateTransformMatrix(const Vec3& position, const Vec3& rotation, const Vec3& scale);

		static inline Mat4 CreateTransformMatrix(const Vec3& position, const Quat& rotation, const Vec3& scale);

		static inline Mat4 CreateTranslationMatrix(const Vec3& translation);

		static inline Mat4 CreateScaleMatrix(const Vec3& scale);

		static inline Mat4 CreateXRotationMatrix(float angle);

		static inline Mat4 CreateYRotationMatrix(float angle);

		static inline Mat4 CreateZRotationMatrix(float angle);

		static inline Mat4 CreateRotationMatrix(const Vec3& rotation);

		static inline Mat4 CreatePerspectiveProjectionMatrix(float, float, float);

		inline Quat ToQuaternion();

		inline Vec3 GetPosition() const;

		inline Quat GetRotation();

		inline Vec3 GetEulerRotation();

		inline Vec3 GetScale() const;

		inline Mat4 CreateInverseMatrix();

		inline Mat4 CreateAdjMatrix();

		inline Mat4 GetCofactor(int p, int q, int n);

		inline float GetDeterminant(int n) const;

		inline Mat4 TransposeMatrix() const;

		inline void Print() const;

		inline std::string ToString() const;

	};

	class Quat
	{
	public:
		float x;
		float y;
		float z;
		float w;

		Quat() : x(0), y(0), z(0), w(1) {}

		Quat(float a) : x(a), y(a), z(a), w(a) {}

		Quat(float a, float b, float c, float d = 1) : x(a), y(b), z(c), w(d) {}

		Quat(const Vec3& a) : x(a.x), y(a.y), z(a.z), w(1.f) {}

		Quat(const Vec4& a) : x(a.x), y(a.y), z(a.z), w(a.w) {}

		inline Quat operator+(const Quat& a) const;

		inline Quat operator-(const Quat& a) const;

		inline Quat operator*(const Quat& a) const;

		inline Quat operator*(const float& a) const;

		inline Vec3 operator*(const Vec3& a) const;

		inline bool operator!=(const Quat& q) const;

		inline float& operator[](const size_t index);

		static inline Quat Identity() { return Quat(0, 0, 0, 1); }

		static inline Quat AngleAxis(float angle, Vec3 axis);

		static inline Quat LookRotation(Vec3 forward, Vec3 up);

		static inline Quat SLerp(const Quat& a, const Quat& b, float time);

		inline void Inverse();

		inline Quat GetInverse() const;

		inline void Normalize();

		inline Quat GetNormal() const;

		inline void Conjugate();

		inline Quat GetConjugate() const;

		inline float Dot(const Quat& a) const;

		inline Vec3 ToEuler() const;

		inline Mat4 ToRotationMatrix() const;

		inline void Print() const;

		inline std::string ToString() const;

	};
}

#include "Math.inl"