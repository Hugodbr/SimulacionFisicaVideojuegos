#include "HurricaneForce.h"

#include "ParticleWithMass.h"
#include "ParticleSystem.h"

HurricaneForce::HurricaneForce(const Region& region, const physx::PxVec3 &eye, const physx::PxVec3 &velocity)
    : WindRegionForce(region, velocity)
    , _eye(eye)
{
}

HurricaneForce::HurricaneForce(const ParticleSystem *particleSystem, const Region& region, const physx::PxVec3 &eye, const physx::PxVec3 &velocity)
    : WindRegionForce(particleSystem, region, velocity)
    , _eye(eye)
{
}

physx::PxVec3 HurricaneForce::computeForceOnParticle(ParticleWithMass &particle)
{
    _windVelocity = getVelocityAtPosition(particle.getPosition());

    return WindForce::computeForceOnParticle(particle);
}

void HurricaneForce::updateForce(double deltaTime)
{
    std::cout << "HurricaneForce ID " << _id << " updating force. No change over time." << std::endl;
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
    return rotatedVel * _k1 * _windSpeed;
}
