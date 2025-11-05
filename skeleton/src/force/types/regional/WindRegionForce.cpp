#include "WindRegionForce.h"

#include <cassert>

#include "ParticleWithMass.h"


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

void WindRegionForce::updateField(double deltaTime)
{
    if (_follows) {
        if (!_followTargetParticle->isActive()) {
            this->setDead();
        } else {
            // Move region to follow target particle
            assert(_region.type == CYLINDER && "WindRegionForce follow particle only implemented for CYLINDER region type.");
            _region.shape.cylinder.update(_followTargetParticle->getPosition(), _region.shape.cylinder.baseCenter, _region.shape.cylinder.radius);
        }
    }
}

void WindRegionForce::updateForce(double deltaTime)
{
    ForceField::updateForce(deltaTime);
}

physx::PxVec3 WindRegionForce::getForceOnParticle(ParticleWithMass &particle)
{
    return computeForceOnParticle(particle);
}

physx::PxVec3 WindRegionForce::computeForceOnParticle(ParticleWithMass &particle)
{
    if (isParticleInsideRegion(particle)) {
        return WindForce::computeForceOnParticle(particle);
    }
    return physx::PxVec3(0.0f, 0.0f, 0.0f);
}
