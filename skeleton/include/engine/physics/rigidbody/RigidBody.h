#pragma once

#include <PxPhysicsAPI.h>
#include "PhysicalObject.h"

class PhysicsEngine;

class RigidBody : public PhysicalObject
{
public:
    RigidBody();
    virtual ~RigidBody();


protected:
    // Static members shared across all RigidBody instances
    static inline PhysicsEngine* _physicsEngine;

    static inline physx::PxPhysics*  _physics  = nullptr;
    static inline physx::PxScene*    _scene    = nullptr;
    
    // Specifics for RigidBody
    physx::PxMaterial* _material = nullptr;
    physx::PxShape*    _shape    = nullptr;
    
};


// class SphereBody: public RigidBody
// {
// public:
//     SphereBody(float size) : _physxSphereGeo(PhysxSphereGeometry(size)) {};
//     virtual ~SphereBody() {};

//     virtual Geometry* getPhysicalGeometry() override { return &_physxSphereGeo; };

// private:
//     PhysxSphereGeometry _physxSphereGeo;
// };


// class BoxBody: public RigidBody
// {
// public:
//     BoxBody(float length) : _physxCubeGeo(PhysxCubeGeometry(length)) {};
//     virtual ~BoxBody() {};

//     virtual Geometry* getPhysicalGeometry() override { return &_physxCubeGeo; };

// private:
//     PhysxCubeGeometry _physxCubeGeo;
// };