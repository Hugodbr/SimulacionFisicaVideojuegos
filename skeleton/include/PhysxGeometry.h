#pragma once

#include "PxPhysicsAPI.h"
#include "Geometry.h"

class PhysixGeometry : public virtual Geometry
{
public:
    PhysixGeometry() {};

    virtual ~PhysixGeometry() = 0;

protected:
    physx::PxGeometryType::Enum _physxGeoType = physx::PxGeometryType::eINVALID;
};

class PhysxSphereGeometry 
    : public virtual SphereGeometry, public virtual PhysixGeometry
{
public:
    PhysxSphereGeometry(float radius);
    virtual ~PhysxSphereGeometry() = default;

    float getRadius() { return _radius; };

protected:
    physx::PxGeometryType::Enum _physxGeoType = physx::PxGeometryType::eSPHERE;
};

class PhysxCubeGeometry 
    : public virtual CubeGeometry, public virtual PhysixGeometry
{
public:
    PhysxCubeGeometry(float length);
    virtual ~PhysxCubeGeometry() = default;

    float getLength() { return _length; };

protected:
    physx::PxGeometryType::Enum _physxGeoType = physx::PxGeometryType::eBOX;
};
