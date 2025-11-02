#pragma once

#include "ForceField.h"

class GlobalForce : public ForceField
{
public:
    GlobalForce(const physx::PxVec3 &direction, float magnitude);
    virtual ~GlobalForce();

    virtual void setMagnitude(float magnitude);
    virtual void setForce(const physx::PxVec3& force);
    virtual void setForceDirection(const physx::PxVec3& direction);

    virtual const physx::PxVec3& getForce() const;

    virtual void updateForce(double deltaTime) override;

protected:
    physx::PxVec3 _direction;
    float _magnitude;
};
