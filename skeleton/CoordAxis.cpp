#include "CoordAxis.h"

#include "RenderUtils.hpp"


CoordAxis::CoordAxis()
{
	// Create all axis and origin
	createRenderItem(physx::PxVec3(0, 0, 0) * _scale, _o, physx::PxVec4(255, 255, 255, 255));
	createRenderItem(physx::PxVec3(1, 0, 0) * _scale, _x, physx::PxVec4(255, 0, 0, 255));
	createRenderItem(physx::PxVec3(0, 1, 0) * _scale, _y, physx::PxVec4(0, 255, 0, 255));
	createRenderItem(physx::PxVec3(0, 0, 1) * _scale, _z, physx::PxVec4(0, 0, 255, 255));
}

CoordAxis::~CoordAxis()
{
	deregisterAll();
}

void CoordAxis::createRenderItem(const physx::PxVec3 &pos, RenderItem* ri, const physx::PxVec4 &color)
{
	physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(_size));
	physx::PxTransform* transform = new physx::PxTransform(pos);
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
