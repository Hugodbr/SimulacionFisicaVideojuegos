#pragma once

#include "WindForce.h"
#include "RegionalForce.h"

// Wind and Region combined with multiple inheritance (virtual to avoid diamond problem)
// Example: a wind blowing only inside a region, like a special power area
class WindRegionForce : public WindForce, public RegionalForce
{
public:
    WindRegionForce(const Region& region, const physx::PxVec3& velocity);
    WindRegionForce(const ParticleSystem* particleSystem, const Region& region, const physx::PxVec3& velocity);
    ~WindRegionForce() = default;

    virtual void init(const Region& region, const physx::PxVec3& velocity);

    virtual void updateForce(double deltaTime) override;
    virtual void applyForceOnParticle(ParticleWithMass& particle) override;

};