#pragma once

#include "WindForce.h"

class HurricaneForce : public WindForce 
{
public:

    HurricaneForce(const physx::PxVec3 &eye, const physx::PxVec3& velocity);
    virtual ~HurricaneForce() = default;

    virtual physx::PxVec3 computeForceOnParticle(Particle& particle) override;
    virtual void updateForce(double deltaTime) override;

    virtual physx::PxVec3 getVelocityAtPosition(const physx::PxVec3& position);

protected:
    physx::PxVec3 _eye;
};