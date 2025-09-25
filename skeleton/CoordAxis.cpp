#include "CoordAxis.h"

#include "RenderUtils.hpp"

#include "Vector3D.h"


CoordAxis::CoordAxis()
{
	// Create all axis and origin
	createRenderItem(Vector3D::Zero()    * _scale, _o, physx::PxVec4(255, 255, 255, 255));
	createRenderItem(Vector3D::Right()   * _scale, _x, physx::PxVec4(0, 0, 255, 255));
	createRenderItem(Vector3D::Up()      * _scale, _y, physx::PxVec4(255, 0, 0, 255));
	createRenderItem(Vector3D::Forward() * _scale, _z, physx::PxVec4(0, 255, 0, 255));
}

CoordAxis::~CoordAxis()
{
	deregisterAll();
}

void CoordAxis::createRenderItem(Vector3D& v, RenderItem* ri, const physx::PxVec4&color)
{
	physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(_size));
	physx::PxTransform* transform = new physx::PxTransform(v.getPxVec3());
	ri = new RenderItem(shape, transform, color);
	RegisterRenderItem(ri);
}

void CoordAxis::deregisterAll()
{
	DeregisterRenderItem(_o);
	DeregisterRenderItem(_x);
	DeregisterRenderItem(_y);
	DeregisterRenderItem(_z);
}
