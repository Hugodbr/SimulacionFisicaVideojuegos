#include "GravitationalForce.h"

#include "ParticleWithMass.h"


GravitationalForce::GravitationalForce()
    : GlobalForce(physx::PxVec3(0.0f, -1.0f, 0.0f), 0.0f) // Temporary values
    , _gravitationalConstant(Constants::Physics::GravitationalConstant)
    , _planetaryMass(Constants::Physics::EarthMass)
    , _planetaryRadius(Constants::Physics::EarthRadius)
{
    _direction = physx::PxVec3(0.0f, -1.0f, 0.0f); // Default direction towards negative Y
    _force = _direction.getNormalized() * _gravitationalConstant * _planetaryMass / (_planetaryRadius * _planetaryRadius);
}

void GravitationalForce::updateForce(double deltaTime)
{
    // Constant over time.
}

void GravitationalForce::applyForceOnParticle(ParticleWithMass &particle)
{
    // Apply gravitational force to the particle
    particle.applyForce(_force);
}

void GravitationalForce::setForceDirection(const physx::PxVec3 &direction)
{
    _direction = direction.getNormalized();
}
