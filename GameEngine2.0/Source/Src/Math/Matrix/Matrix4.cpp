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

int Matrix4::GetDeterminant(int n)
{
	Matrix4 a;
	int D = 0; // Initialize result

	//  Base case : if matrix contains single element
	if (n == 1)
		return (int)content[0][0];

	int sign = 1;  // To store sign multiplier

	 // Iterate for each element of first row
	for (int f = 0; f < n; f++)
	{
		// Getting Cofactor of matrix[0][f]
		a = GetCofactor(0, f, n);
		D += sign * (int)content[0][f] * a.GetDeterminant(n - 1);

		// terms are to be added with alternate sign
		sign = -sign;
	}

	return D;
}

Matrix4 Matrix4::CreateInverseMatrix()
{
	// Find determinant of matrix
	Matrix4 inverse;
	int det = GetDeterminant(4);
	if (det == 0)
	{
		return 1;
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