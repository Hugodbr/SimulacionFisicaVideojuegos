#include "HurricaneForce.h"

#include "ParticleWithMass.h"
#include "ParticleSystem.h"

HurricaneForce::HurricaneForce(const Region& region, const physx::PxVec3 &eye, const physx::PxVec3 &velocity)
    : WindRegionForce(region, velocity)
    , _eye(eye)
{ }

HurricaneForce::HurricaneForce(const ParticleSystem *particleSystem, const Region& region, const physx::PxVec3 &eye, const physx::PxVec3 &velocity)
    : WindRegionForce(particleSystem, region, velocity)
    , _eye(eye)
{ }

void HurricaneForce::updateField(double deltaTime)
{
    if (this->isActive()) {
        std::cout << "HurricaneForce ID " << _id << " active." << std::endl;
        WindRegionForce::updateField(deltaTime);
    }
}

void HurricaneForce::updateForce(double deltaTime)
{
    WindRegionForce::updateForce(deltaTime);
}

physx::PxVec3 HurricaneForce::computeForceOnParticle(ParticleWithMass &particle)
{
    // Update wind velocity at particle position so WindForce can compute correctly!
    _windVelocity = getVelocityAtPosition(particle.getPosition());

    return WindRegionForce::computeForceOnParticle(particle);
}

physx::PxVec3 HurricaneForce::getVelocityAtPosition(const physx::PxVec3 &position)
{
    physx::PxVec3 local = position - _eye;

    physx::PxVec3 localVel(
        -local.z,
        (50 - local.y),
        local.x
    );

    return _k1 * localVel * _windSpeed;
}
