#pragma once

#include "ForceField.h"

class Particle;

class WindForce : public ForceField
{
public:

    WindForce(const physx::PxVec3& velocity);
    virtual ~WindForce() = default;

    virtual void setSpeed(float speed);
    virtual void setDirection(const physx::PxVec3& direction);
    virtual void setVelocity(const physx::PxVec3& velocity);
    virtual void setFrictionCoefficient(float coefficient);

    virtual physx::PxVec3 computeForceOnParticle(Particle& particle) override;
    virtual void updateForce(double deltaTime) override;

protected:
    physx::PxVec3 _windDirection;
    float _windSpeed;
    physx::PxVec3 _windVelocity;

    float _frictionCoefficient;
    float _k1; // Linear drag coefficient
    float _k2; // Turbulent drag coefficient
};