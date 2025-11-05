#pragma once

#include "GlobalForce.h"

// Default for earth-like gravity
// F = G * (m1 * m2) / r^2
// For planetary body of mass gravity simulation
// Downward force in y direction
class GravitationalForce : public GlobalForce
{
public:
    GravitationalForce();
    ~GravitationalForce() = default;

    virtual void updateForce(double deltaTime) override;

    // Override to prevent external setting of force parameters
    virtual void setMagnitude(float magnitude) override {}
    virtual void setForce(const physx::PxVec3& force) override {}
    virtual void setForceDirection(const physx::PxVec3& direction) override;

    virtual void setGravitationalConstant(float gConst) { _gravitationalConstant = gConst; }
    virtual void setPlanetaryMass(float mass) { _planetaryMass = mass; }
    virtual void setPlanetaryRadius(float radius) { _planetaryRadius = radius; }

protected:
    virtual physx::PxVec3 computeForceOnParticle(ParticleWithMass& particle) override;

private:
    float _gravitationalConstant; // Gravitational constant G
    float _planetaryMass; // e.g., mass of the Earth
    float _planetaryRadius; // e.g., radius of the Earth

    physx::PxVec3 _forceByMass; // Direction of the gravitational force
};
