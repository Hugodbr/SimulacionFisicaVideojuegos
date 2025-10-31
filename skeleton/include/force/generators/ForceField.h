#pragma once

#include "ForceGenerator.h"

class Particle;

class ForceField : public ForceGenerator
{
public:
    ForceField();
    virtual ~ForceField() = default;

    virtual void updateForce(double deltaTime) override;
    virtual physx::PxVec3 computeForceOnParticle(Particle& particle);
};