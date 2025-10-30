#pragma once

#include "ForceField.h"

// Default for earth-like gravity
// F = G * (m1 * m2) / r^2
// For planetary body of mass gravity simulation
class GravitationalForce : public ForceField
{
public:
    GravitationalForce();
    ~GravitationalForce() = default;

    virtual void setMagnitude(float magnitude) override {}
    virtual void setForce(const physx::PxVec3& force) override {}
    virtual void setForceDirection(const physx::PxVec3& direction) override;

    virtual void setGravitationalConstant(float gConst) { _gravitationalConstant = gConst; }
    virtual void setPlanetaryMass(float mass) { _planetaryMass = mass; }
    virtual void setPlanetaryRadius(float radius) { _planetaryRadius = radius; }

private:
    float _gravitationalConstant; // Gravitational constant G
    float _planetaryMass; // e.g., mass of the Earth
    float _planetaryRadius; // e.g., radius of the Earth
};
