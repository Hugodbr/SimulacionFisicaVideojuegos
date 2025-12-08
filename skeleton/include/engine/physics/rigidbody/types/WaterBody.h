#pragma once

#include "RigidBody.h"


class WaterBody : public RigidBody
{
public:
    WaterBody(const physx::PxBounds3& bounds);
    virtual ~WaterBody() = default;

protected:
    physx::PxRigidStatic* _body = nullptr;
};