#include "Include/Math/Math.h"

using namespace Math;

Matrix4::Matrix4(float diagonal)
{
	for (size_t i = 0; i < 4; i++) content[i][i] = diagonal;
}

Matrix4::Matrix4(const Matrix4& in)
{
	for (size_t j = 0; j < 4; j++)
	{
		for (size_t i = 0; i < 4; i++)
		{
			content[j][i] = in.content[j][i];
		}
	}
}

Matrix4::Matrix4(const float* data)
{
	for (size_t j = 0; j < 4; j++)
	{
		for (size_t i = 0; i < 4; i++)
		{
			content[j][i] = data[j * 4 + i];
		}
	}
}
Math::Matrix4::Matrix4(const double* data)
{
	for (size_t j = 0; j < 4; j++)
	{
		for (size_t i = 0; i < 4; i++)
		{
			content[j][i] = (float)data[j * 4 + i];
		}
	}
}

Matrix4 Matrix4::Multiply(Matrix4 a) const
{
	Matrix4 out;
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

Matrix4 Matrix4::operator*(const Matrix4& in) const
{
	Matrix4 out;
	for (size_t j = 0; j < 4; j++)
	{
		for (size_t i = 0; i < 4; i++)
		{
			float res = 0;
			for (size_t k = 0; k < 4; k++)
				res += content[j][k] * in.content[k][i];

			out.content[j][i] = res;
		}
	}
	return out;
}

Matrix4 Matrix4::operator+(const Matrix4& a) const
{
	Matrix4 tmp;
	for (size_t j = 0; j < 4; j++)
	{
		for (size_t i = 0; i < 4; i++)
		{
			tmp.content[j][i] = content[j][i] + a.content[j][i];
		}
	}
	return tmp;
}

float& Matrix4::operator[](const size_t in)
{
	return content[in / 4][in % 4];
}

float& Matrix4::at(const unsigned char x, const unsigned char y)
{
	if (x > 3 || y > 3)
		return content[0][0];
	return content[y][x];
}

Vector4 Matrix4::operator*(const Vector4& in) const
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

Matrix4 Matrix4::CreateXRotationMatrix(float angle)
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

Matrix4 Matrix4::CreateYRotationMatrix(float angle)
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

Matrix4 Matrix4::CreateZRotationMatrix(float angle)
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

Matrix4 Matrix4::CreateTranslationMatrix(const Vector3& translation)
{
	Matrix4 out = Matrix4(1);
	for (size_t i = 0; i < 3; i++) out.at(3, (const unsigned char)i) = translation[i];
	return out;
}

Matrix4 Matrix4::CreateRotationMatrix(const Vector3& rotation)
{
	return CreateYRotationMatrix(rotation.y) * CreateXRotationMatrix(rotation.x) * CreateZRotationMatrix(rotation.z);
}

Matrix4 Matrix4::CreateScaleMatrix(const Vector3& scale)
{
	Matrix4 out;
	for (size_t i = 0; i < 3; i++) out.at((const unsigned char)i, (const unsigned char)i) = scale[i];
	out.content[3][3] = 1;
	return out;
}

Matrix4 Matrix4::CreatePerspectiveProjectionMatrix(float Near, float Far, float fov)
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

Matrix4 Matrix4::CreateTransformMatrix(const Vector3& position, const Vector3& rotation, const Vector3& scale)
{
	return CreateTranslationMatrix(position) * CreateYRotationMatrix(rotation.y) * CreateXRotationMatrix(rotation.x) * CreateZRotationMatrix(rotation.z) * CreateScaleMatrix(scale);
}

Matrix4 Matrix4::CreateTransformMatrix(const Vector3& position, Quaternion rotation, const Vector3& scale)
{
	return CreateTranslationMatrix(position) * rotation.ToRotationMatrix() * CreateScaleMatrix(scale);
}


Vector3 Matrix4::GetPosition()
{
	return Vector3(content[0][3], content[1][3], content[2][3]);
}

Quaternion Matrix4::GetRotation()
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

	return rotMat.ToQuaternion();
}

Vector3 Matrix4::GetEulerRotation()
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

Vector3 Matrix4::GetScale()
{
	// World Scale equal lenght of columns of the model matrix.
	float x = Vector3(content[0][0], content[0][1], content[0][2]).GetLength();
	float y = Vector3(content[1][0], content[1][1], content[1][2]).GetLength();
	float z = Vector3(content[2][0], content[2][1], content[2][2]).GetLength();
	return Vector3(x, y, z);
}

const Quaternion Matrix4::ToQuaternion()
{
	float w = sqrtf(1 + at(0, 0) + at(1, 1) + at(2, 2)) / 2;
	return Quaternion((at(2, 1) - at(1, 2)) / (4 * w), (at(0, 2) - at(2, 0)) / (4 * w), (at(1, 0) - at(0, 1)) / (4 * w), w);
}

Matrix4 Matrix4::GetCofactor(int p, int q, int n)
{
	Matrix4 mat;
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

float Matrix4::GetDeterminant(int n)
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

Matrix4 Matrix4::CreateInverseMatrix()
{
	// Find determinant of matrix
	Matrix4 inverse(1);
	float det = GetDeterminant(4);
	if (det == 0.0f)
	{
		return Matrix4(1);
	}

	// Find adjoint
	Matrix4 adj = CreateAdjMatrix();

	// Find Inverse using formula "inverse(A) = adj(A)/det(A)"
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			inverse.content[i][j] = adj.content[i][j] / float(det);

	return inverse;
}

Matrix4 Matrix4::CreateAdjMatrix()
{
	// temp is used to store cofactors of matrix
	Matrix4 temp;
	Matrix4 adj;
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


Matrix4 Matrix4::TransposeMatrix()
{
	float x[16] = { 0 };
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			x[i + j * 4] = content[i][j];
		}
	}

	return Matrix4{ x };
}

void Matrix4::PrintMatrix()
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

Matrix4 Matrix4::Identity()
{
	return Matrix4(1);
}