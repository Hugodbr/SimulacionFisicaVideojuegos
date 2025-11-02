#pragma once

#include "RegionalForce.h"

class ExplosionForce : public RegionalForce
{
public:
    // Construct a RegionalForce globally (not tied to a ParticleSystem)
    // The region is used to limit the area of effect of the explosion
    ExplosionForce(const Region& region, const physx::PxVec3& center, float maxRadius, float magnitude, float expansionVelocity = 10.0f);
    // Construct a RegionalForce tied to a ParticleSystem
    ExplosionForce(ParticleSystem* particleSystem, const Region& region, const physx::PxVec3& center, float maxRadius, float magnitude, float expansionVelocity);

    virtual ~ExplosionForce() = default;

    virtual void updateForce(double deltaTime) override;
    virtual void updateMagnitude(double deltaTime);
    virtual void updateRadius();

    virtual physx::PxVec3 computeForceAtPosition(const physx::PxVec3& position);
    virtual physx::PxVec3 computeForceOnParticle(ParticleWithMass& particle) override;
    virtual void applyForceOnParticle(ParticleWithMass& particle) override;

    void setCenter(const physx::PxVec3& center) { _center = center; }
    void setRadius(float radius) { _radius = radius; }
    void setMagnitude(float magnitude) { _magnitude = magnitude; }
    void setExpansionVelocity(const physx::PxVec3& velocity) { _expansionVelocity = velocity; }

    physx::PxVec3 getCenter() const { return _center; }
    float getRadius() const { return _radius; }
    float getMagnitude() const { return _magnitude; }

protected:
    physx::PxVec3 _center;
    float _maxRadius;
    float _radius;
    float _initialMagnitude;
    float _magnitude;
    double _timeConstant;
    double _timeElapsed;
    physx::PxVec3 _expansionVelocity;
};