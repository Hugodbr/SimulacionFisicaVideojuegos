#pragma once

#include "ForceField.h"

// Base class for forces that apply uniformly to all particles in the system
class GlobalForce : public ForceField
{
public:
    GlobalForce();
    GlobalForce(const physx::PxVec3 &direction, float magnitude);
    virtual ~GlobalForce() = default;

    virtual void updateForce(double deltaTime) override;

protected:
    virtual physx::PxVec3 computeForceOnParticle(ParticleWithMass& particle);

    virtual void updateField(double deltaTime);

protected:
    physx::PxVec3 _direction;
    float _magnitude;
};
