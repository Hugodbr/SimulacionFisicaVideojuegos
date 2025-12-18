#pragma once

#include "ForcePair.h"

class RigidBody;
class ParticleWithMass;

class SpringForce : public ForcePair
{
public:
    SpringForce(
        PhysicalObject* mainObject, 
        PhysicalObject* otherObject, 
        float k, 
        float restingLength, 
        float maxLength
    );
    virtual ~SpringForce();

    virtual void setTensionOnly(bool tensionOnly) { _tensionOnly = tensionOnly; }

    virtual void updateForce(double deltaTime) override;
    // virtual void setK(double k) { _k = k; };
    // virtual void setRestingLength(double restingLength) { _restingLength = restingLength; };
    // virtual void setMaxLength(double maxLength) { _maxLength = maxLength; };

    virtual physx::PxVec3 getForceOnRigidBody(RigidBody& rigidBody);
    virtual physx::PxVec3 getForceOnParticle(ParticleWithMass& particle);

protected:
    virtual physx::PxVec3 computeForceOnRigidBody(RigidBody& rigidBody);
    virtual physx::PxVec3 computeForceOnParticle(ParticleWithMass& particle);

protected:
    float _k;
    float _restingLength;
    float _maxLength;
    bool _tensionOnly = false;

    float _maxForce; // To prevent extreme forces
};