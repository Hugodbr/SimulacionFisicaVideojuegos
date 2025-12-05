#pragma once

#include "ForceField.h"

class ExplosionForce : public ForceField
{
public:
    // Construct a ForceField globally (not tied to a ParticleSystem)
    ExplosionForce(const physx::PxVec3& center, float maxRadius, float magnitude, float expansionVelocity = 1000.0f);
    // Construct a ForceField tied to a ParticleSystem
    ExplosionForce(ParticleSystem* particleSystem, const physx::PxVec3& center, float maxRadius, float magnitude, float expansionVelocity = 1000.0f);
    virtual ~ExplosionForce() = default;

    virtual void updateForce(double deltaTime) override;

    void setCenter(const physx::PxVec3& center) { _center = center; }
    void setRadius(float radius) { _radius = radius; }
    void setMagnitude(float magnitude) { _magnitude = magnitude; }
    void setExpansionVelocity(float velocity) { _expansionVelocity = velocity; }

    physx::PxVec3 getCenter() const { return _center; }
    float getRadius() const { return _radius; }
    float getMagnitude() const { return _magnitude; }

protected:
    virtual void init(const physx::PxVec3& center, float maxRadius, float magnitude, float expansionVelocity);

    virtual void updateMagnitude();
    virtual void updateRadius();

    virtual physx::PxVec3 computeForceAtPosition(const physx::PxVec3& position);
    virtual physx::PxVec3 computeForceOnParticle(ParticleWithMass& particle) override;

    virtual void updateField(double deltaTime) override;

protected:
    physx::PxVec3 _center;
    float _maxRadius;
    float _radius;

    float _initialMagnitude;
    float _magnitude;

    double _timeConstant;
    double _timeElapsed;

    float _expansionVelocity;

    double _timer;
};