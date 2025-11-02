#include "GlobalForce.h"

GlobalForce::GlobalForce(const physx::PxVec3 &direction, float magnitude)
    : ForceField()
    , _direction(direction.getNormalized())
    , _magnitude(magnitude)
{
    _force = _direction * _magnitude;
}

GlobalForce::~GlobalForce()
{
}

void GlobalForce::setMagnitude(float magnitude)
{
    _magnitude = magnitude;
    _force = _direction * _magnitude;
}

void GlobalForce::setForce(const physx::PxVec3& force)
{
    _force = force;
    _magnitude = force.magnitude();
    if (!_force.isZero()) {
        _direction = force.getNormalized();
    }
    else {
        _direction = physx::PxVec3(0.0f, 0.0f, 0.0f); // Zero direction
    }
}

void GlobalForce::setForceDirection(const physx::PxVec3 &direction)
{
    _direction = direction.getNormalized();
    _force = _direction * _magnitude;
}

const physx::PxVec3& GlobalForce::getForce() const
{
    return _force;
}

void GlobalForce::updateForce(double deltaTime)
{
    std::cout << "GlobalForce ID " << _id 
            << " applying force: (" << _force.x << ", " << _force.y << ", " << _force.z << ")" 
            << " with magnitude: " << _magnitude 
            << " and direction: (" << _direction.x << ", " << _direction.y << ", " << _direction.z << ")" 
            << std::endl;
}
