#pragma once

#include "ForceField.h"

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
    virtual void setK1(float k1) { _k1 = k1; }
    virtual void setK2(float k2) { _k2 = k2; }

    // Override to prevent external setting of force parameters of the setters bellow:
    virtual void setMagnitude(float magnitude) override {}
    virtual void setForceDirection(const physx::PxVec3& direction) override {}
    virtual void setForce(const physx::PxVec3& force) override {}
    
    virtual void updateForce(double deltaTime) override;
    
protected:
    virtual physx::PxVec3 computeForceOnParticle(ParticleWithMass& particle) override;
    virtual physx::PxVec3 computeForceOnRigidBody(RigidBody& rigidBody) override;

    virtual void updateField(double deltaTime) override;

protected:
    float _frictionCoefficient = 0.1f;
    float _k1; // Linear drag coefficient
    float _k2; // Turbulent drag coefficient

    physx::PxVec3 _windDirection;
    float _windSpeed;
    physx::PxVec3 _windVelocity;
};