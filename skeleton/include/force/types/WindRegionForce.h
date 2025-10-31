#pragma once

#include "WindForce.h"

class WindRegionForce : public WindForce
{
public:
    WindRegionForce(const physx::PxVec3& direction, float strength, const physx::PxBounds3& region);
    ~WindRegionForce() = default;

    // void applyForce(Particle& particle) override;

private:
    physx::PxVec3 _direction;
    float _strength;
    physx::PxBounds3 _region;
};