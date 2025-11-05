#pragma once

#include "ForceField.h"
#include "Region.h"

class ParticleWithMass;

// ! ONLY BOX REGION IS IMPLEMENTED NOW
// RegionalForce applies a force only within a defined region
class RegionalForce : virtual public ForceField
{
public:
    // Construct a RegionalForce globally (not tied to a ParticleSystem)
    RegionalForce(const Region& region);
    // Construct a RegionalForce tied to a ParticleSystem
    RegionalForce(const ParticleSystem* particleSystem, const Region& region);
    virtual ~RegionalForce() = default;

    virtual void updateForce(double deltaTime) override;

    virtual void setRegion(const Region& region);
    virtual void setFollowParticle(bool follow, const ParticleWithMass& particle);

protected:
    virtual bool isParticleInsideRegion(const ParticleWithMass& particle);

    // Return zero vector if outside region
    virtual physx::PxVec3 computeForceOnParticle(ParticleWithMass& particle) override;

    virtual void updateField(double deltaTime) override;

protected:
    Region _region;

    bool _follows;
    const ParticleWithMass* _followTargetParticle;
};