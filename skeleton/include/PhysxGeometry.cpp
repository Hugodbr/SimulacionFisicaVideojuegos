#include "PhysxGeometry.h"


PhysixGeometry::~PhysixGeometry()
{
}

PhysxSphereGeometry::PhysxSphereGeometry(float radius)
    : SphereGeometry(radius)
{
};

PhysxCubeGeometry::PhysxCubeGeometry(float length)
    : CubeGeometry(length)
{
}
