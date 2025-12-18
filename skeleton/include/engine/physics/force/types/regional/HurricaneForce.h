#pragma once

#include "WindRegionForce.h"


class HurricaneForce : public WindRegionForce
{
public:
    HurricaneForce(const Region& region, const physx::PxVec3 &eye, const physx::PxVec3& velocity);
    HurricaneForce(const ParticleSystem *particleSystem, const Region& region, const physx::PxVec3 &eye, const physx::PxVec3& velocity);
    virtual ~HurricaneForce() = default;

    virtual void updateForce(double deltaTime) override;

protected:
    virtual physx::PxVec3 getVelocityAtPosition(const physx::PxVec3& position);

    virtual physx::PxVec3 computeForceOnParticle(ParticleWithMass& particle) override;
    virtual physx::PxVec3 computeForceOnRigidBody(RigidBody& rigidBody) override;

    virtual void updateField(double deltaTime) override;

protected:
    physx::PxVec3 _eye;
};