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
    physx::PxVec3 local = position - _eye;

    // Swirl pattern in local coordinates
    physx::PxVec3 localVel(
        -local.z,
        (50 - local.y),
        local.x
    );

    // Rotation from +Z to wind direction
    physx::PxQuat rotation = physx::PxShortestRotation(physx::PxVec3(0, 0, 1), _windVelocity.getNormalized());
    
    // Rotate field to align with wind direction
    physx::PxVec3 rotatedVel = rotation.rotate(localVel);

    // Apply intensity and return
    return _windSpeed * rotatedVel;
    // return 1 * physx::PxVec3(-(position.z - _eye.z), (50 - (position.y - _eye.y)), (position.x - _eye.x));
}
