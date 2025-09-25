#include "Vector3D.h"


Vector3D::Vector3D(const physx::PxVec3& v) {
	_v.x = v.x;
	_v.y = v.y;
	_v.z = v.z;
}

Vector3D::Vector3D() {}

Vector3D::Vector3D(float x, float y, float z) : Vector3D(physx::PxVec3(x, y, z)) {}


Vector3D Vector3D::Zero()
{
	return Vector3D();
}

Vector3D Vector3D::One()
{
	return Vector3D(1, 1, 1);
}

Vector3D Vector3D::Forward()
{
	return Vector3D(0, 0, 1);
}

Vector3D Vector3D::Back()
{
	return Vector3D(0, 0, -1);
}

Vector3D Vector3D::Left()
{
	return Vector3D(-1, 0, 0);
}

Vector3D Vector3D::Right()
{
	return Vector3D(1, 0, 0);
}

Vector3D Vector3D::Up()
{
	return Vector3D(0, 1, 0);
}

Vector3D Vector3D::Down()
{
	return Vector3D(0, -1, 0);
}

float Vector3D::getX() const
{
	return _v.x;
}

float Vector3D::getY() const
{
	return _v.y;
}

float Vector3D::getZ() const
{
	return _v.z;
}

physx::PxVec3& Vector3D::getPxVec3()
{
	return this->_v;
}

void Vector3D::setX(float x)
{
	_v.x = x;
}

void Vector3D::setY(float y)
{
	_v.y = y;
}

void Vector3D::setZ(float z)
{
	_v.z = z;
}

void Vector3D::set(float x, float y, float z)
{
	_v = physx::PxVec3(x, y, z);
}

float Vector3D::magnitude() const 
{
	return _v.magnitude();
}

void Vector3D::normalize() 
{
	_v = _v.getNormalized();
}

Vector3D& Vector3D::operator=(const Vector3D& v)
{
	_v = physx::PxVec3(v.getX(), v.getY(), v.getZ());
	return *this;
}

Vector3D Vector3D::operator+(const Vector3D& v) const
{
	return Vector3D();
}

Vector3D Vector3D::operator-(const Vector3D& v) const
{
	return Vector3D();
}

Vector3D Vector3D::operator*(float s) const
{
	return Vector3D(_v.x * s, _v.y * s, _v.z * s);
}

float Vector3D::operator*(const Vector3D& v) const
{
	return _v.x * v.getX() + _v.y * v.getY() + _v.z * v.getZ();
}

Vector3D Vector3D::operator/(float s) const
{
	return Vector3D(_v.x / s, _v.y / s, _v.z / s);
}