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

    virtual void updateForce(double deltaTime) override;
    virtual physx::PxVec3 getForceOnParticle(ParticleWithMass& particle) override;

protected:
    virtual void init(const Region& region, const physx::PxVec3& velocity);

    // Return zero vector if outside region
    virtual physx::PxVec3 computeForceOnParticle(ParticleWithMass& particle) override;

    virtual void updateField(double deltaTime) override;

};