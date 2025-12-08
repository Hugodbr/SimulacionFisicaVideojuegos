#include "engine/physics/rigidbody/RigidBody.h"

#include "PhysicsEngine.h"
#include "RigidBody.h"


RigidBody::RigidBody()
    : PhysicalObject(
        physx::PxTransform(physx::PxVec3(0,0,0), physx::PxQuat(0, physx::PxVec3(0,0,0))), // default transform
        physx::PxVec3(0,0,0), // default velocity
        Constants::Particle::Default::Size, // default size
        Constants::Physics::Damping, // default damping
        Constants::Color::White, // default color
        Constants::Integration_Method::VERLET
    )
{
    _physicsEngine = &PhysicsEngine::getInstance();

    _physics  = _physicsEngine->getPhysics();
    _scene    = _physicsEngine->getScene();
}

RigidBody::~RigidBody()
{
    _shape->release();
    _material->release();
}
