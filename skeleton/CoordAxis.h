#pragma once

#include <PxPhysicsAPI.h>


class RenderItem;
class Vector3D;

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

	void createRenderItem(Vector3D& v, RenderItem* ri, const physx::PxVec4& color);

	void deregisterAll();
};

