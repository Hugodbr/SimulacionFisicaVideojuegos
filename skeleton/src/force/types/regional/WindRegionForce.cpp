#include "WindRegionForce.h"


WindRegionForce::WindRegionForce(const Region& region, const physx::PxVec3& velocity)
    : ForceGenerator()
    , ForceField()
    , WindForce(velocity)
    , RegionalForce(region)
{
    init(region, velocity);
}

WindRegionForce::WindRegionForce(const ParticleSystem* particleSystem, const Region& region, const physx::PxVec3& velocity)
    : ForceGenerator(particleSystem)
    , ForceField(particleSystem)
    , WindForce(particleSystem, velocity)
    , RegionalForce(particleSystem, region)
{
    init(region, velocity);
}

void WindRegionForce::init(const Region &region, const physx::PxVec3 &velocity)
{
    _windVelocity = velocity;
    _windDirection = _windVelocity.getNormalized();
    _windSpeed = _windVelocity.magnitude();
}

void WindRegionForce::updateForce(double deltaTime)
{
    WindForce::updateForce(deltaTime);
}

void WindRegionForce::applyForceOnParticle(ParticleWithMass& particle)
{
    RegionalForce::applyForceOnParticle(particle);
}