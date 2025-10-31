#include "HurricaneForce.h"

#include "Particle.h"

HurricaneForce::HurricaneForce(const physx::PxVec3 &eye, const physx::PxVec3 &velocity)
    : WindForce(velocity)
    , _eye(eye)
{
}

physx::PxVec3 HurricaneForce::computeForceOnParticle(Particle &particle)
{
    _windVelocity = getVelocityAtPosition(particle.getPosition());

    return WindForce::computeForceOnParticle(particle);
}

void HurricaneForce::updateForce(double deltaTime)
{
}

physx::PxVec3 HurricaneForce::getVelocityAtPosition(const physx::PxVec3 &position)
{
    return 1 * physx::PxVec3(-(position.z - _eye.z), (50 - (position.y - _eye.y)), (position.x - _eye.x));
}
