#pragma once

#include "ForceField.h"
#include "Region.h"

class ParticleWithMass;

// ! ONLY BOX REGION IS IMPLEMENTED NOW
class RegionalForce : virtual public ForceField
{
public:
    // Construct a RegionalForce globally (not tied to a ParticleSystem)
    RegionalForce(const Region& region);
    // Construct a RegionalForce tied to a ParticleSystem
    RegionalForce(const ParticleSystem* particleSystem, const Region& region);

    virtual ~RegionalForce() = default;

    virtual void updateForce(double deltaTime) override;
    virtual void applyForceOnParticle(ParticleWithMass& particle) override;

protected:
    virtual bool isParticleInsideRegion(const ParticleWithMass& particle);

    Region _region;
};