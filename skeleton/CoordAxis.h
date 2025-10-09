#pragma once
#include <PxPhysicsAPI.h>

class RenderItem;

class CoordAxis
{
private:
	RenderItem* _o = nullptr; // origin
	RenderItem* _x = nullptr;
	RenderItem* _y = nullptr;
	RenderItem* _z = nullptr;

	const float _size = 1;
	const float _scale = 10;

public:
	CoordAxis();

	~CoordAxis();

	void createRenderItem(const physx::PxVec3 &pos, RenderItem* ri, const physx::PxVec4 &color);

	void deregisterAll();
};

