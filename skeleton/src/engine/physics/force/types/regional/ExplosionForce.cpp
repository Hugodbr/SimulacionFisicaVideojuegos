#include "ExplosionForce.h"

#include <math.h>

#include "ParticleWithMass.h"


ExplosionForce::ExplosionForce(const physx::PxVec3 &center, float maxRadius, float magnitude, float expansionVelocity)
    :  ForceField()
{  
    init(center, maxRadius, magnitude, expansionVelocity);
}

ExplosionForce::ExplosionForce(ParticleSystem *particleSystem, const physx::PxVec3 &center, float maxRadius, float magnitude, float expansionVelocity)
    : ForceField(particleSystem)
{
    init(center, maxRadius, magnitude, expansionVelocity);
}

void ExplosionForce::init(const physx::PxVec3 &center, float maxRadius, float magnitude, float expansionVelocity)
{
    _center = center;
    _maxRadius = maxRadius;
    _initialMagnitude = magnitude;
    _magnitude = magnitude;
    _expansionVelocity = expansionVelocity;

    _timeConstant = 3.0;
    _timeElapsed = 0.0;
    _radius = 0.0f;
    _timer = 5.0; // seconds before explosion occurs
}

void ExplosionForce::updateMagnitude()
{
    _magnitude = exp(-1.0 * (_timeElapsed / _timeConstant)) * _initialMagnitude;

    if (_magnitude < 1.0f) {
        // Magnitude too low, mark as dead
        this->setDead();
    }
}

void ExplosionForce::updateRadius()
{
    if (_radius < _maxRadius) {
        _radius = _expansionVelocity * static_cast<float>(_timeElapsed);
    }
    else {
        // Max radius reached, mark as dead
        this->setDead();
    }
}

void ExplosionForce::updateField(double deltaTime)
{
    if (this->isActive()) {
        std::cout << "ExplosionForce ID " << _id << " active exploding." << std::endl;
        
        _timeElapsed += deltaTime; // To update magnitude and radius over time

        updateMagnitude();
        updateRadius();
    }
}

void ExplosionForce::updateForce(double deltaTime)
{
    ForceField::updateForce(deltaTime);
}

physx::PxVec3 ExplosionForce::computeForceAtPosition(const physx::PxVec3 &position)
{
    physx::PxVec3 direction = position - _center;
    float distance = direction.magnitude();

    if (distance < _radius && distance > 0.0f) {
        direction = direction.getNormalized();
        _force = (_magnitude * direction) / (distance * distance);
    }
    else {
        _force = physx::PxVec3(0.0f, 0.0f, 0.0f);
    }

    return _force;
}

physx::PxVec3 ExplosionForce::computeForceOnParticle(ParticleWithMass &particle)
{
    return computeForceAtPosition(particle.getPosition());
}
