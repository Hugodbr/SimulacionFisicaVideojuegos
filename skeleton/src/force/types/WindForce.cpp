#include "WindForce.h"

#include "Constants.h"
#include "ParticleWithMass.h"


WindForce::WindForce(const physx::PxVec3 &velocity)
    : ForceField()
{
    setVelocity(velocity);
    setFrictionCoefficient(_frictionCoefficient); // Default friction coefficient
}

WindForce::WindForce(const ParticleSystem *particleSystem, const physx::PxVec3 &velocity)
    : ForceField(particleSystem)
{
    setVelocity(velocity);
    setFrictionCoefficient(_frictionCoefficient); // Default friction coefficient
}

void WindForce::setSpeed(float speed)
{
    _windSpeed = speed;
    _windVelocity = _windDirection * _windSpeed;
}

void WindForce::setDirection(const physx::PxVec3 &direction)
{
    _windDirection = direction.getNormalized();
    _windVelocity = _windDirection * _windSpeed;
}

void WindForce::setVelocity(const physx::PxVec3 &velocity)
{
    _windVelocity = velocity;
    _windSpeed = _windVelocity.magnitude();

    if (_windSpeed > Constants::Math::epsilon) {
        _windDirection = _windVelocity.getNormalized();
    } else {
        _windDirection = physx::PxVec3(0.0f, 0.0f, 0.0f);
    }
}

void WindForce::setFrictionCoefficient(float coefficient)
{
    _frictionCoefficient = coefficient;
    // Simple model: linear and quadratic drag coefficients based on friction coefficient
    _k1 = _frictionCoefficient;          // Linear drag
    _k2 = 0.5f * _frictionCoefficient;   // Turbulent drag
}

void WindForce::updateField(double deltaTime)
{
}

void WindForce::updateForce(double deltaTime)
{
    ForceField::updateForce(deltaTime);
}

physx::PxVec3 WindForce::computeForceOnParticle(ParticleWithMass& particle)
{
    physx::PxVec3 particleVelocity = particle.getVelocity();
    // Reset force. Its different for each particle.
    _force = physx::PxVec3(0.0f, 0.0f, 0.0f);

    // Calculate relative velocity between wind and particle
    physx::PxVec3 relativeVelocity = _windVelocity - particleVelocity;

    // Compute drag force using linear and quadratic components
    _force = _k1 * relativeVelocity +
             _k2 * relativeVelocity.magnitude() * relativeVelocity;

    return _force;
}
