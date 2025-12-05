#pragma once

#include "PhysicalObject.h"
#include "PhysxGeometry.h"


class RigidBody : public PhysicalObject
{
public:
    RigidBody();
    virtual ~RigidBody() = default;

    virtual Geometry* getPhysicalGeometry() = 0;
};


class SphereBody: public RigidBody
{
public:
    SphereBody(float size) : _physxSphereGeo(PhysxSphereGeometry(size)) {};
    virtual ~SphereBody() {};

    virtual Geometry* getPhysicalGeometry() override { return &_physxSphereGeo; };

private:
    PhysxSphereGeometry _physxSphereGeo;
};


class BoxBody: public RigidBody
{
public:
    BoxBody(float length) : _physxCubeGeo(PhysxCubeGeometry(length)) {};
    virtual ~BoxBody() {};

    virtual Geometry* getPhysicalGeometry() override { return &_physxCubeGeo; };

private:
    PhysxCubeGeometry _physxCubeGeo;
};