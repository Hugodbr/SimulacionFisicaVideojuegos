#pragma once

#include "ForceField.h"

class ParticleWithMass;

class WindForce : virtual public ForceField
{
public:

    WindForce(const physx::PxVec3& velocity);
    WindForce(const ParticleSystem* particleSystem, const physx::PxVec3& velocity);
    virtual ~WindForce() = default;

    virtual void setSpeed(float speed);
    virtual void setDirection(const physx::PxVec3& direction);
    virtual void setVelocity(const physx::PxVec3& velocity);
    virtual void setFrictionCoefficient(float coefficient);

    
    virtual void updateForce(double deltaTime) override;
    // virtual void applyForceOnParticle(ParticleWithMass& particle) override;
    
protected:

    virtual physx::PxVec3 computeForceOnParticle(ParticleWithMass& particle) override;

protected:
    float _frictionCoefficient = 0.1f;
    float _k1; // Linear drag coefficient
    float _k2; // Turbulent drag coefficient

    physx::PxVec3 _windDirection;
    float _windSpeed;
    physx::PxVec3 _windVelocity;
};