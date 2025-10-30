#include "ForceGenerator.h"

#include "ForceManager.h"

// Initialize static member
uint64_t ForceGenerator::_nextId = 0;

ForceGenerator::ForceGenerator()
    : _id(_nextId++)
    , _direction(physx::PxVec3(0.0f, 0.0f, 0.0f))
    , _magnitude(0.0f)
    , _forceManager(ForceManager::getInstance())
    , _force(physx::PxVec3(0.0f, 0.0f, 0.0f))
{
}

void ForceGenerator::setMagnitude(float magnitude) {
    _magnitude = magnitude;
}

void ForceGenerator::setForce(const physx::PxVec3 &force) {
    _force = force;
}

void ForceGenerator::setForceDirection(const physx::PxVec3 &direction) {
    physx::PxVec3 dirNormalized = direction.getNormalized();
    _force = dirNormalized * _magnitude;
}