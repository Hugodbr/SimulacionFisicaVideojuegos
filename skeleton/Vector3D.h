#pragma once

#include <PxPhysicsAPI.h>
#include <cmath>


class Vector3D
{
private:
	physx::PxVec3 _v = physx::PxVec3(0);

	Vector3D(const physx::PxVec3& v);

public:

	/* --- Static Properties--- */
	// Using UNITY documentation as inspiration:

	// Shorthand for writing Vector3(0, 0, 0)
	static Vector3D Zero();
	// Shorthand for writing Vector3(1, 1, 1)
	static Vector3D One();

	// Shorthand for writing Vector3(0, 0, 1)
	static Vector3D Forward();
	// Shorthand for writing Vector3(0, 0, -1)
	static Vector3D Back();

	// Shorthand for writing Vector3(-1, 0, 0)
	static Vector3D Left();
	// 	Shorthand for writing Vector3(1, 0, 0)
	static Vector3D Right();

	// Shorthand for writing Vector3(0, 1, 0)
	static Vector3D Up();
	// Shorthand for writing Vector3(0, -1, 0)
	static Vector3D Down();


	/* --- Constructors --- */

	// Default constructor: vector zero
	Vector3D();

	Vector3D(float x, float y, float z);

	~Vector3D() = default;

	/* --- Methods --- */

	// Observers
	float getX() const;
	float getY() const;
	float getZ() const;

	physx::PxVec3& getPxVec3();
	
	// 
	void setX(float x);
	void setY(float y);
	void setZ(float z);

	void set(float x, float y, float z);

	/* Operations */

	// Magnitude
	float magnitude() const;

	// vector in the same direction of length 1
	void normalize();


	/* Operators */

	// Copy assignment
	Vector3D& operator=(const Vector3D& v);

	// Vector addition
	Vector3D operator+(const Vector3D& v) const;

	// Vector subtraction
	Vector3D operator-(const Vector3D& v) const;

	// Product by scalar
	Vector3D operator*(float s) const;

	// Scalar product
	float operator *(const Vector3D& v) const;

	// Division by constant (scaling)
	Vector3D operator/(float s) const;
};

