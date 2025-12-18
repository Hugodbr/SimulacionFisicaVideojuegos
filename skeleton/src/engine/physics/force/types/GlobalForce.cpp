#include "GlobalForce.h"

#include "ParticleWithMass.h"

GlobalForce::GlobalForce()
    : ForceField()
    , _direction(physx::PxVec3(0.0f, 0.0f, 0.0f))
    , _magnitude(0.0f)
{ }

GlobalForce::GlobalForce(const physx::PxVec3 &direction, float magnitude)
    : ForceField(), _direction(direction.getNormalized()), _magnitude(magnitude)
{
    _force = _direction * _magnitude;
}

void GlobalForce::updateField(double deltaTime)
{
    // Default implementation does nothing.
}

void GlobalForce::updateForce(double deltaTime)
{
    updateField(deltaTime);

    std::cout << "GlobalForce ID " << _id 
            << " applying force: (" << _force.x << ", " << _force.y << ", " << _force.z << ")" 
            << " with magnitude: " << _magnitude 
            << " and direction: (" << _direction.x << ", " << _direction.y << ", " << _direction.z << ")" 
            << std::endl;
}

physx::PxVec3 GlobalForce::computeForceOnParticle(ParticleWithMass &particle)
{
    return _force;
}

physx::PxVec3 GlobalForce::computeForceOnRigidBody(RigidBody &rigidBody)
{
    return _force;
}
