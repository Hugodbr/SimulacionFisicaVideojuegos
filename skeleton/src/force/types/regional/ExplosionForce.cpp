#include "ExplosionForce.h"

#include <math.h>

#include "ParticleWithMass.h"


ExplosionForce::ExplosionForce(const Region &region, const physx::PxVec3 &center, float maxRadius, float magnitude, float expansionVelocity)
    :  RegionalForce(region)
    , _center(center)
    , _maxRadius(maxRadius)
    , _radius(0.0f)
    , _initialMagnitude(magnitude)
    , _magnitude(magnitude)
    , _timeConstant(3.0)
    , _timeElapsed(0.0)
    , _expansionVelocity(expansionVelocity)
{   
}

ExplosionForce::ExplosionForce(ParticleSystem *particleSystem, const Region &region, const physx::PxVec3 &center, float maxRadius, float magnitude, float expansionVelocity)
    : RegionalForce(particleSystem, region)
    , _center(center)
    , _maxRadius(maxRadius)
    , _radius(0.0f)
    , _initialMagnitude(magnitude)
    , _magnitude(magnitude)
    , _timeConstant(3.0)
    , _timeElapsed(0.0)
    , _expansionVelocity(expansionVelocity)
{
}

void ExplosionForce::updateForce(double deltaTime)
{
    _timeElapsed += deltaTime;
    updateMagnitude(deltaTime);
    updateRadius();

    // Mark as dead if magnitude is too low. The explosion has finished and the force can be removed.
    if (_magnitude < 0.001f) {
        this->setDead(true);
    }
}

void ExplosionForce::updateMagnitude(double deltaTime)
{
    _magnitude = exp(-1.0 * (_timeElapsed / _timeConstant)) * _initialMagnitude;
}

void ExplosionForce::updateRadius()
{
    if (_radius < _maxRadius) {
        _radius = _expansionVelocity.magnitude() * static_cast<float>(_timeElapsed);
    }
}

physx::PxVec3 ExplosionForce::computeForceAtPosition(const physx::PxVec3 &position)
{
    physx::PxVec3 direction = position - _center;
    float distance = direction.magnitude();

    if (distance < _radius && distance > 0.0f) {
        direction = direction.getNormalized();
        _force = (_magnitude * direction) / distance;
    }

    if (distance >= _radius || distance == 0.0f || _magnitude < 0.001f) {
        _force = physx::PxVec3(0.0f, 0.0f, 0.0f);
    }

    return _force;
}

physx::PxVec3 ExplosionForce::computeForceOnParticle(ParticleWithMass &particle)
{
    return computeForceAtPosition(particle.getPosition());
}

void ExplosionForce::applyForceOnParticle(ParticleWithMass &particle)
{
    // Compute force at particle position
    physx::PxVec3 force = computeForceOnParticle(particle);
    
    // Apply force only if inside region
    RegionalForce::applyForceOnParticle(particle);
}
