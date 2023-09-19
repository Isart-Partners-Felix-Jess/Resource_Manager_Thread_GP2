#pragma once

#include <vector>
#include <utility>
#include <cassert>

#include "vectorM.hpp"

template <typename T, size_t M, size_t N = M>
class MatrixMN
{
	static_assert(M > 0 && N > 0, "M and N must be greater than 0");
private:
	bool square = false;
	std::vector<VectorM<T, N>> data;

public:
#pragma region Constructors
	// Null by default
	// Identity fills last rows by 0 if more rows than cols
	MatrixMN(bool identity = false)
	{
		if (M == N)
			square = true;
		data.resize(M);
		for (VectorM<T, N>& row : data)
			for (size_t j = 0; j < N; j++)
				row[j] = 0;
		if (identity)
			for (size_t i = 0; i < N; i++)
				data[i][i] = 1;
	}

	// Constructor for initializing with {{,...}, ...}
	MatrixMN(std::initializer_list<std::initializer_list<T>> values)
	{
		if (M == N)
			square = true;
		data.resize(M);
		size_t i = 0, j = 0;
		for (const auto& row : values) {
			for (const auto& val : row) {
				if (i >= M || j >= N)
					break; // Ignore excess values
				data[i][j++] = val;
			}
			i++;
			j = 0;
		}
		// Fill remaining elements with 0
		for (; i < M; i++) {
			for (; j < N; j++)
				data[i][j] = 0;
			j = 0;
		}
	}

	MatrixMN(VectorM<T, N>* vectors)
	{
		if (M == N)
			square = true;
		data.resize(M);
		for (size_t i = 0; i < M; i++)
			data[i] = vectors[i];
	}

	// Copy into another matrix(0 for uninitialized)
	template <typename T, size_t P, size_t Q>
	MatrixMN(const MatrixMN<T, P, Q>& other)
	{
		if (M == N)
			square = true;
		data.resize(M);
		for (size_t i = 0; i < M; i++)
			for (size_t j = 0; j < N; j++) {
				if (i < P && j < Q)
					data[i][j] = other[i][j];
				else
					data[i][j] = 0;
			}
	}
#pragma endregion //Constructors

#pragma region Properties
	bool IsSquare() {
		return square;
	}

	bool IsDiagonal()
	{
		if (!square)
			return false;
		for (int i = 1; i < M; i++)
			for (int j = 1; j < M; j++)
				if (i != j && data[i][j])
					return false;
		return true;
	}

	VectorM<T, std::min(M, N)> diagonal()
	{
		VectorM<T, std::min(M, N)> diagonal;
		for (size_t i = 0; i < std::min(M, N); i++)
			diagonal[i] = data[i][i];
		return diagonal;
	}

	T trace()
	{
		T temp = 0;
		for (size_t i = 0; i < std::min(M, N); i++)
			temp += data[i][i];
		return temp;
	}

	// Faster for Matrix 3x3 or less
	T DeterminantRecursive() const
	{
		static_assert(M == N, "Matrix must be square");
		if constexpr (M <= static_cast <size_t>(0))
			return 0;
		else if constexpr (M <= static_cast <size_t> (1))
			return data[0][0];
		else if constexpr (M <= static_cast<size_t>(2))
			return data[0][0] * data[1][1] - data[0][1] * data[1][0];
		else
		{
			T det = 0;
			int sign = 1;

			for (size_t colFirstRow = 0; colFirstRow < M; colFirstRow++)
			{
				// Subroutine
				MatrixMN<T, M - 1 > submatrix;
				for (size_t i = 1; i < M; i++)
				{
					for (size_t j = 0, k = 0; j < M; j++)
						if (j != colFirstRow)
						{
							submatrix[i - 1][k] = data[i][j];
							k++;
						}
					T temp = submatrix.DeterminantRecursive();
					det += sign * data[0][colFirstRow] * temp;
				}
				sign = -sign;
			}
			return det;
		}
	}

	// Faster than recursive and can give the determinant for the MatrixMN<T,M,M>
	// (actual determinant of MatrixMN doesn't exist in this case)
	T determinant() const
	{
		MatrixMN<T, M, N> temp(*this);
		return temp.GaussianAlgorithm();
	}
#pragma endregion //Properties

#pragma region Transformations
	MatrixMN<T, N, M> Transposed() const
	{
		MatrixMN<T, N, M> result;
		for (size_t i = 0; i < N; i++)
			result[i] = Column(i);
		return result;
	}

	// Augment to the right
	template<size_t P>
	MatrixMN<T, M, (N + P)> Augment(MatrixMN<T, M, P> other) const
	{
		MatrixMN<T, M, (N + P)>result(*this);
		for (size_t i = 0; i < M; i++)
		{
			for (size_t j = 0; j < P; j++)
				result[i][N + j] = other[i][j];
		}
		return result;
	}

	// Not const, applies directly on the matrix
	T GaussianAlgorithm(bool debug = false)
	{
		T det = 1;
		int r = 0;
		for (int j = 0; j < N; ++j)
		{
			T max = 0;
			T pivot = 0;
			int maxId = 0;
			for (int i = r; i < M; ++i)
			{
				if (abs(data[i][j]) > max)
				{
					pivot = data[i][j];
					max = abs(pivot);
					maxId = i;
				}
			}
			if (abs(data[maxId][j]) <= 0.001f)
			{
				data[maxId][j] = 0.0f;
				// NULL pivot
				det = 0;
			}
			else if (pivot)
			{
				for (int k = r; k < N; ++k)
				{
					data[maxId][k] /= pivot;
				}
				if (maxId != r)
				{
					std::swap(data[maxId], data[r]);
					det = -det;
					if (debug)
					{
						std::cout << "\nSwap ! ->" << j << std::endl;
						Print();
					}
				}
				for (int i = 0; i < M; ++i)
				{
					if (i != r)
					{
						// Save the first element of the row
						T firstElement = data[i][j];
						for (int p = j; p < N; ++p)
						{
							data[i][p] -= data[r][p] * firstElement;
						}
					}
					if (abs(data[i][j]) <= 0.001f)
						data[i][j] = 0.0f;
				}
				if (debug)
				{
					std::cout << "\nSubstract ->" << j << std::endl;
					Print();
				}
				det *= pivot;
				r++;
			}
			// No pivot;
			else
				det = 0;
		}
		return det;
	}

	MatrixMN<T, M> Inversion() const
	{
		// Check if Matrix is squared and determinant is Null
		if (!square || !this->determinant())
		{
			std::cout << "Matrix is not inversible" << std::endl;
			return *this;
		}
		MatrixMN<T, M> identityM(true);
		// Copy and Augment by identityM
		MatrixMN<T, M, M + M> temp = this->Augment(identityM);
		temp.GaussianAlgorithm();
		// Extract last part
		MatrixMN < T, M> result;
		for (size_t i = 0; i < M; i++)
			for (size_t j = 0; j < M; j++)
				result[i][j] = temp[i][M + j];
		return result;
	}
#pragma endregion //Transformations

#pragma region GettersAndPrint
	void Print()
	{
		for (VectorM<T, N>& row : data)
		{
			std::cout << " ( ";
			for (size_t j = 0; j < N; j++)
				std::cout << row[j] << " ";
			std::cout << ")\n";
		}
		std::cout << std::endl;
	}

	VectorM<T, M> Column(size_t index) const
	{
		if (index >= N)
			throw std::runtime_error("outside of range");
		VectorM<T, M> Column;
		for (size_t i = 0; i < M; i++)
			Column[i] = data[i][index];
		return Column;
	}

	size_t GetRowsNb() {
		return N;
	}

	size_t GetColumnsNb() {
		return M;
	}
#pragma endregion //GettersAndPrint

#pragma region Operators
	VectorM<T, N>& operator[](size_t index)
	{
		assert(index < M && "Index must be inside vector range (inferior to its dimension)");
		return data[index];
	}

	const VectorM<T, N>& operator[](size_t index) const
	{
		assert(index < M && "Index must be inside vector range (inferior to its dimension)");
		return data[index];
	}

	// Assignment
	template<size_t P, size_t Q>
	MatrixMN<T, M, N>& operator=(const MatrixMN<T, P, Q>& other)
	{
		if (M == N)
			square = true;
		for (size_t i = 0; i < std::min(P, M); i++)
			this->data[i] = other[i];

		if (P < M) {
			// Use MatrixMN constructor to initialize remaining elements to 0
			MatrixMN<T, M - P, Q> zeros;
			for (size_t i = P; i < M; i++)
				this->data[i] = zeros[i - P];
		}
		return *this;
	}

	// Mathematical operators
	MatrixMN<T, M, N> operator+(const MatrixMN<T, M, N>& other) const
	{
		MatrixMN<T, M, N> result(*this);
		for (size_t i = 0; i < M; i++)
			result.data[i] = data[i] + other[i];
		return result;
	}

	MatrixMN& operator+=(const MatrixMN& other)
	{
		*this = (*this) + other;
		return *this;
	}

	MatrixMN<T, M, N> operator-() const
	{
		MatrixMN<T, M, N> result(*this);
		for (size_t i = 0; i < M; i++)
			result.data[i] = -data[i];
		return result;
	}

	MatrixMN<T, M, N> operator-(const MatrixMN<T, M, N>& other) const
	{
		MatrixMN<T, M, N> result(*this);
		for (size_t i = 0; i < M; i++)
			result[i] = data[i] - other[i];
		return result;
	}

	MatrixMN < T, M, N>& operator-=(const MatrixMN& other)
	{
		*this = (*this) - other;
		return *this;
	}

	// Product by a scalar
	MatrixMN operator*(const T& scalar) const
	{
		MatrixMN<T, M, N> result(*this);
		for (size_t i = 0; i < M; i++)
			result[i] = data[i] * scalar;
		return result;
	}

	MatrixMN < T, M, N> operator*=(const T& scalar)
	{
		*this = (*this) * scalar;
		return *this;
	}

	// Product by a matrix
	template <size_t P, size_t Q>
	MatrixMN<T, M, Q> operator*(const MatrixMN<T, P, Q>& other) const {
		static_assert(N == P, "Matrix1 must have the same number of Columns than Matrix2 has of rows");
		MatrixMN<T, M, Q> result;
		for (size_t i = 0; i < M; i++)
			for (size_t j = 0; j < Q; j++) {
				VectorM<T, Q> col = other.Column(j);
				result[i][j] = data[i].Dot(col);
			}
		return result;
	}

	template <size_t P, size_t Q>
	MatrixMN<T, M, Q>& operator*=(const MatrixMN<T, P, Q>& other)
	{
		*this = *this * other;
		return *this;
	}
};
#pragma endregion //Operators

// Product vector by matrix
template <typename T, size_t M, size_t N>
VectorM<T, M> operator*(const MatrixMN<T, M, N>& matrix, const VectorM<T, N>& vector)
{
	VectorM<T, M> result;
	for (size_t j = 0; j < M; j++)
		result[j] = matrix[j].Dot(vector);
	return result;
}

template <typename T>
VectorM<T, 3> operator*(const MatrixMN<T, 4>& matrix, const VectorM<T, 3>& vector)
{
	VectorM<T, 3> result;
	for (size_t i = 0; i < 3; i++)
		result[i] = matrix[i].Dot({ vector[0],vector[1],vector[2], 1.f }) + matrix[i][3];
	return result;
}
template <typename T>
VectorM<T, 4> operator*(const MatrixMN<T, 4>& matrix, const VectorM<T, 4>& vector)
{
	VectorM<T, 4> result;
	for (size_t i = 0; i < 4; i++)
		result[i] = matrix[i].Dot(vector);
	return result;
}

// Order reversed cause of assignment
template <typename T>
VectorM<T, 3> operator*=(const VectorM<T, 3>& vector, const MatrixMN<T, 4>& matrix) {
	return matrix * vector;
}

template <typename T>
void Swap(std::vector<T>& a, std::vector<T>& b)
{
	std::vector<T> temp = a;
	a = b;
	b = temp;
}

namespace matrix
{
	template <typename T>
	MatrixMN<T, 2> Scale2D(VectorM<T, 2> alphabeta)
	{
		MatrixMN<T, 2> result(true);
		result[0][0] = alphabeta[0];
		result[1][1] = alphabeta[1];
		return result;
	}

	template <typename T>
	MatrixMN<T, 3> Scale3D(VectorM<T, 3> alphabetagamma)
	{
		MatrixMN<T, 3> result(true);
		result[0][0] = alphabetagamma[0];
		result[1][1] = alphabetagamma[1];
		result[2][2] = alphabetagamma[2];
		return result;
	}

	template <typename T>
	MatrixMN<T, 2> Rotate2D(T angleinrad)
	{
		T ca;
		T sa;
		if (angleinrad)
		{
			ca = std::cos(angleinrad);
			sa = std::sin(angleinrad);
			T tolerance = 1e-5;
			if (abs(ca) <= tolerance)
				ca = 0;
			if (abs(sa) <= tolerance)
				sa = 0;
		}
		MatrixMN<T, 2> result(true);
		result[0][0] = ca;  result[0][1] = -sa;
		result[1][0] = sa;  result[1][1] = std::cos(angleinrad);
		return result;
	}

	enum class Axis
	{
		X,
		Y,
		Z
	};

	template <typename T>
	MatrixMN<T, 3> Rotate3D(T angleinrad, Axis axis)
	{
		MatrixMN<T, 3> result(true);
		T ca;
		T sa;
		if (angleinrad)
		{
			ca = std::cos(angleinrad);
			sa = std::sin(angleinrad);
			T tolerance = (T)1e-5;
			if (abs(ca) <= tolerance)
				ca = 0;
			if (abs(sa) <= tolerance)
				sa = 0;
		}
		else
		{
			ca = 1;
			sa = 0;
		}
		switch (axis)
		{
		case matrix::Axis::X:
			result[0][0] = 1;
			result[1][1] = ca;  result[1][2] = -sa;
			result[2][1] = sa;  result[2][2] = ca;
			break;
		case matrix::Axis::Y:
			result[0][0] = ca;   result[0][2] = sa;
			result[1][1] = 1;
			result[2][0] = -sa;  result[2][2] = ca;
			break;
		case matrix::Axis::Z:
			result[0][0] = ca;  result[0][1] = -sa;
			result[1][0] = sa;  result[1][1] = ca;
			result[2][2] = 1;
			break;
		default:
			std::cout << "not a valid Axis" << std::endl;
			break;
		}
		return result;
	}

	// Faster, don't compute sin or cos if null angle
	template<typename T>
	MatrixMN<T, 3> Rotate3dAllAxis(VectorM<T, 3> angleXYZinrad)
	{
		MatrixMN<T, 3> rotationXYZ(true);
		const T tolerance = (T)1e-5;

		T cosA = 0; T sinA = 0; T cosB = 0; T sinB = 0; T cosC = 0; T sinC = 0;

		if (angleXYZinrad[0])
		{
			cosA = (T)std::cos(angleXYZinrad[0]);
			cosA = (std::abs(cosA) < tolerance) ? (T)0.0 : cosA;

			sinA = (T)std::sin(angleXYZinrad[0]);
			sinA = (std::abs(sinA) < tolerance) ? (T)0.0 : sinA;
		}
		else
		{
			cosA = 1; sinA = 0;
		}

		if (angleXYZinrad[1])
		{
			cosB = (T)std::cos(angleXYZinrad[1]);
			cosB = (std::abs(cosB) < tolerance) ? (T)0.0 : cosB;

			sinB = (T)std::sin(angleXYZinrad[1]);
			sinB = (std::abs(sinB) < tolerance) ? (T)0.0 : sinB;
		}
		else
		{
			cosB = 1; sinB = 0;
		}

		if (angleXYZinrad[2])
		{
			cosC = (T)std::cos(angleXYZinrad[2]);
			cosC = (std::abs(cosC) < tolerance) ? (T)0.0 : cosC;

			sinC = (T)std::sin(angleXYZinrad[2]);
			sinC = (std::abs(sinC) < tolerance) ? (T)0.0 : sinC;
		}
		else
		{
			cosC = 1; sinC = 0;
		}
		rotationXYZ[0][0] = cosB * cosC;
		rotationXYZ[1][0] = -cosB * sinC;
		rotationXYZ[2][0] = sinB;
		rotationXYZ[0][1] = sinA * sinB * cosC + cosA * sinC;
		rotationXYZ[1][1] = -sinA * sinB * sinC + cosA * cosC;
		rotationXYZ[2][1] = -sinA * cosB;
		rotationXYZ[0][2] = -cosA * sinB * cosC + sinA * sinC;
		rotationXYZ[1][2] = cosA * sinB * sinC + sinA * cosC;
		rotationXYZ[2][2] = cosA * cosB;
		return rotationXYZ;
	}

	template <typename T>
	MatrixMN<T, 4> MatrixTRS(VectorM<T, 3> translation, VectorM<T, 3> angleXYZinrad, VectorM<T, 3> scaleFactors)
	{
		// Set rotation values

		// Faster XYZ
		MatrixMN<T, 3> rotationXYZ = Rotate3dAllAxis(angleXYZinrad);

		// Apply scaling to the Columns
		for (size_t i = 0; i < 3; i++)
			for (size_t j = 0; j < 3; j++)
				rotationXYZ[i][j] *= scaleFactors[j];

		// Extend Matrix
		MatrixMN<T, 4>result = rotationXYZ;
		result[3][3] = 1;

		// Set translation values
		result[0][3] = translation[0];
		result[1][3] = translation[1];
		result[2][3] = translation[2];

		return result;
	}

	// Fast access
	template <typename T>
	MatrixMN<T, 4> MatrixTRS(T transX, T transY, T transZ, T rotaX, T rotaY, T rotaZ, T scaleX, T scaleY, T scaleZ)
	{
		// Create the three vectors from the input parameters
		VectorM<T, 3> translation = { transX, transY, transZ };
		VectorM<T, 3> rotation = { rotaX, rotaY, rotaZ };
		VectorM<T, 3> scaling = { scaleX, scaleY, scaleZ };

		return MatrixTRS(translation, rotation, scaling);
	}
}
typedef MatrixMN<float, 2> Matrix2x2;
typedef MatrixMN<float, 3> Matrix3x3;
typedef MatrixMN<float, 4> Matrix4x4;