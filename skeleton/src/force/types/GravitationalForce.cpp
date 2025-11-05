#include "GravitationalForce.h"

#include "ParticleWithMass.h"


GravitationalForce::GravitationalForce()
    : GlobalForce()
    , _gravitationalConstant(Constants::Physics::GravitationalConstant)
    , _planetaryMass(Constants::Physics::EarthMass)
    , _planetaryRadius(Constants::Physics::EarthRadius)
{
    _direction = physx::PxVec3(0.0f, -1.0f, 0.0f); // Default direction towards negative Y
    // Formula: F/m = G * (PlanetaryMass) / r^2
    _forceByMass = _direction.getNormalized() * _gravitationalConstant * _planetaryMass / (_planetaryRadius * _planetaryRadius);
}

void GravitationalForce::setForceDirection(const physx::PxVec3 &direction)
{
    _direction = direction.getNormalized();
}

void GravitationalForce::updateForce(double deltaTime)
{
    // Constant over time.
}

physx::PxVec3 GravitationalForce::computeForceOnParticle(ParticleWithMass &particle)
{
    return  _forceByMass * particle.getMass();
}
