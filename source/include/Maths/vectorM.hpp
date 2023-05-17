#pragma once
#include <vector>
#include <cassert>
#include <iostream>
#include<math.h>
#include <algorithm>

template <typename T, size_t M = 3>
class VectorM {
	static_assert(M > 0, "M must be greater than 0");
protected:
	size_t dimension = M; // size of vector
	T elements[M]; // content
public:
	VectorM()
	{
		for (size_t i = 0; i < M; i++) {
			elements[i] = 0;
		}
	}

	VectorM(const T(&arr)[M]) { // array constructor
		for (size_t i = 0; i < M; i++) {
			elements[i] = arr[i];
		}
	}
	// array constructor for vector between two points
	VectorM(const T(&arr1)[M], const T(&arr2)[M]) {
		for (size_t i = 0; i < M; i++) {
			elements[i] = arr2[i] - arr1[i];
		}
	} VectorM(std::initializer_list<T> values)
	{
		size_t i = 0;
		for (const auto& val : values) {
			if (i >= M) {
				break; // Ignore excess values
			}
			elements[i++] = val;
		}
		// Fill remaining elements with 0
		for (; i < M; i++) {
			elements[i] = 0;
		}
	}
	// copy constructor for vector between two points
	VectorM(const VectorM<T, M>& v1, const VectorM<T, M>& v2) {
		for (size_t i = 0; i < M; i++) {
			elements[i] = v2[i] - v1[i];
		}
	}
	template <typename... Args,
		typename std::enable_if<sizeof...(Args) == M, int>::type = 0> //Here to avoid conflicts
	VectorM(Args... args) : elements{ static_cast<T>(args)... } {}
	virtual ~VectorM() {
	};
#pragma region Print
	void Print()
	{
		std::cout << "( ";
		for (size_t i = 0; i < dimension; i++)
		{
			std::cout << elements[i];
			std::cout << " ";
		}
		std::cout << ")";
		std::cout << std::endl;
	}
#pragma endregion //Print

#pragma region vecOperations
	//dot Product
	T Dot(const VectorM<T, M>& other) const {
		T result = 0;
		for (size_t i = 0; i < dimension; i++) {
			result += elements[i] * other[i];
		}
		return result;
	}
	VectorM Opposite() const {
		return -(*this);
	}
	T MagnitudeSquared() const {
		return this->Dot(*this);
	}
	T Magnitude() const {
		return sqrt(this->MagnitudeSquared());
	}
	T LengthSquared(const VectorM& other) const {
		VectorM diff = *this - other;
		return diff.MagnitudeSquared();
	}
	T Length(const VectorM& other) const {
		VectorM diff = *this - other;
		return diff.Magnitude();
	}
	VectorM Normalize() const {
		//		assert(this != VectorM.Null());
		return (*this) / this->Magnitude();
	}
#pragma endregion //VecOperations

#pragma region Operators
	//Getters

	T GetDimension()
	{
		return dimension;
	}
	// Direct access to elements
	T& operator[](size_t index) {
		assert(index < M && "Index must be inside vector range (inferior to its dimension)");
		return elements[index];
	}
	const T& operator[](size_t index) const {
		assert(index < dimension && "Index must be inside vector range (inferior to its dimension)");
		return elements[index];
	}
	T(&data())[M] {
		return elements;
	}
		//Assignment
	template<size_t N>
	VectorM<T, M>& operator=(const VectorM<T, N>& other)
	{
		//Stop condition is similar to a std::min
		for (size_t i = 0; i < N && i<M; i++) {
			this->elements[i] = other[i];
		}
		if (N < M) {
			VectorM<T, M - N>zeros;
			for (size_t i = N; i < M; i++) {
				this->elements[i] = zeros[i - N];
			}
		}
		return *this;
	}
	// mathematical operators
	VectorM<T, M> operator+(const VectorM<T, M>& other) const {
		VectorM<T, M> result;
		for (size_t i = 0; i < dimension; i++) {
			result[i] = elements[i] + other[i];
		}
		return result;
	}
	VectorM<T, M>& operator+=(const VectorM<T, M>& other) const {
		*this = (*this) + other;
		return *this;
	}
	VectorM<T, M> operator-() const {
		VectorM<T, M> result;
		for (size_t i = 0; i < dimension; i++) {
			if (elements[i] != 0)
				result[i] = -elements[i];
		}
		return result;
	}
	VectorM<T, M> operator-(const VectorM<T, M>& other) const {
		VectorM<T, M> result;
		for (size_t i = 0; i < dimension; i++) {
			result[i] = elements[i] - other[i];
		}
		return result;
	}
	VectorM<T, M>& operator-=(const VectorM<T, M>& other) const {
		*this = (*this) - other;
		return *this;
	}
	//Product by a scalar
	VectorM<T, M> operator*(const T& scalar) const {
		VectorM<T, M> result;
		for (size_t i = 0; i < dimension; i++) {
			result[i] = elements[i] * scalar;
		}
		return result;
	}
	VectorM<T, M>& operator*=(const T& scalar) const {
		*this = (*this) * scalar;
		return *this;
	}
	//Product between each components
	VectorM<T, M> operator*(const VectorM<T, M>& other) const {
		VectorM<T, M> result;
		for (size_t i = 0; i < dimension; i++) {
			result[i] = elements[i] * other[i];
		}
		return result;
	}
	VectorM<T, M> operator*=(const VectorM<T, M>& other) const {
		return (*this * other);
	}
	VectorM<T, M> operator/(const T& scalar) const {
		//static_assert(scalar != 0, "Can't divide by 0 !");
		VectorM<T, M> result;
		for (size_t i = 0; i < dimension; i++) {
			result[i] = elements[i] / scalar;
		}
		return result;
	}
	VectorM<T, M>& operator/=(const T& scalar) const {
		*this = (*this) / scalar;
		return *this;
	}
	bool operator==(const VectorM<T, M>& other) const {
		for (size_t i = 0; i < M; i++)
		{
			if (this->elements[i] != other.elements[i])
				return false;
		}
		return true;
	}
#pragma endregion //Operators
};

template <typename T>
class Vector2 : public VectorM<T, 2>
{
public:
	typedef Vector2 Point2;
	typedef Vector2 Vec2;
	using VectorM<T, 2>::elements;
	Vector2<T>() : VectorM<T, 2>() {};

	Vector2<T>(T X, T Y) : VectorM<T, 2>(X, Y)
	{
	};
	Vector2<T>(const VectorM<T, 2>& other) : VectorM<T, 2>(other[0], other[1])
	{
	};
	~Vector2<T>() override {};

	T& X() { return elements[0]; };
	T& Y() { return elements[1]; };

	T Cross_product(Vector2<T> v2) const {
		return X() * v2.Y() - Y() * v2.X();
	};
	T GetAngleVector(Vector2<T> v2) const
	{
		T denom = (this->Magnitude() * v2.Magnitude());
		if (!denom)
			return 0;
		T cos = this->Dot(v2) / denom;
		T angle = std::acos(cos);
		if (Cross_product >= 0)
			return angle;
		return -angle;
	};
	Vector2<T>& operator=(const VectorM<T, 2>& other)
	{
		for (size_t i = 0; i < 2; ++i) {
			elements[i] = other[i];
		}
		return *this;
	}
	operator VectorM<T, 2>() const {
		return VectorM<T, 2>(elements[0], elements[1]);
	}
};
typedef Vector2<float> Vectorf2;
typedef Vector2<int> Vectori2;

template <typename T>
class Vector3 : public VectorM<T, 3>
{
public:
	typedef Vector3 Point3;
	typedef Vector3 Vec3;
	using VectorM<T, 3>::elements;
	Vector3<T>() : VectorM<T, 3>() {};

	Vector3<T>(T X, T Y, T Z) : VectorM<T, 3>(X, Y, Z)
	{
	}
	Vector3<T>(const VectorM<T, 3>& other) : VectorM<T, 3>(other[0], other[1], other[2])
	{
	};
	//Vector3<T>(const Vector4<T>& other) : VectorM<T, 3>(other.X(), other.Y(), other.Z) {};
	Vector3<T>(const VectorM<T, 2>& other, T Z = 0) : VectorM<T, 3>(other[0], other[1], Z)
	{
	};
	//Vector3<T>(const Vector2<T>& other, T Z = 0) : Vector3<T>(other.X(), other.Y(), Z)
	//{
	//};
	~Vector3<T>() override {};

	const T& X() const { return elements[0]; };
	const T& Y() const { return elements[1]; };
	const T& Z() const { return elements[2]; };

	Vector3 Cross_product(Vector3<T> v2) const {
		return { Y() * v2.Z() - Z() * v2.Y(),
			Z() * v2.X() - X() * v2.Z(),
			X() * v2.Y() - Y() * v2.X() };
	};
	Vector3 GetNormalVector(Vector3<T> v) const {
		return this->Cross_product(v).Normalize();
	};
	T GetAngleVector(Vector3 v2) const
	{
		T denom = (this->Magnitude() * v2.Magnitude());
		if (!denom)
			return 0;
		T cos = this->Dot(v2) / denom;
		T angle = std::acos(cos);
		return angle;
	};
	Vector3<T>& operator=(const VectorM<T, 3>& other)
	{
		for (size_t i = 0; i < 3; ++i) {
			elements[i] = other[i];
		}
		return *this;
	}
	operator VectorM<T, 3>() const {
		return VectorM<T, 3>(elements[0], elements[1], elements[2]);
	}
};
typedef  Vector3<float> Vectorf3;

template <typename T>
class Vector4 : public VectorM<T, 4>
{
public:
	typedef Vector4 Vec4;
	using VectorM<T, 4>::elements;

	Vector4<T>() : VectorM<T, 4>() {};
	Vector4<T>(T* pos)
	{
		elements[0] = pos[0];
		elements[1] = pos[1];
		elements[2] = pos[2];
		elements[3] = pos[3];
	};
	Vector4<T>(T posX, T posY, T posZ, T posW = 1)
	{
		elements[0] = posX;
		elements[1] = posY;
		elements[2] = posZ;
		elements[3] = posW;
	};
	Vector4<T>(const T& F) {
		elements[0] = F;
		elements[1] = F;
		elements[2] = F;
		elements[3] = F;
	};
	Vector4<T>(Vector2<T>& a, T posZ, T posW)
	{
		elements[0] = a.X();
		elements[1] = a.Y();
		elements[2] = posZ;
		elements[3] = posW;
	}
	Vector4<T>(Vector3<T>& a, T posW)
	{
		elements[0] = a.X();
		elements[1] = a.Y();
		elements[2] = a.Z();
		elements[3] = posW;
	};
	~Vector4() {};

	float& X() { return elements[0]; };
	float& Y() { return elements[1]; };
	float& Z() { return elements[2]; };
	float& W() { return elements[3]; };
};
typedef Vector4<float> Vectorf4;