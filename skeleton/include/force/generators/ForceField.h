#pragma once

#include "ForceGenerator.h"

class ParticleWithMass;

class ForceField : virtual public ForceGenerator
{
public:
    // For global forces, particleSystem can be nullptr
    ForceField();
    ForceField(const ParticleSystem* particleSystem);
    virtual ~ForceField() = default;

    virtual void updateForce(double deltaTime) override;

protected:
    virtual physx::PxVec3 computeForceOnParticle(ParticleWithMass& particle) override;

    virtual void updateField(double deltaTime);
};