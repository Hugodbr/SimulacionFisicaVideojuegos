#pragma once

#include <iostream>

#include <PxPhysicsAPI.h>

#include "ForceManager.h"

class ForceGenerator
{
public:
    ForceGenerator();
    virtual ~ForceGenerator() = default;

    virtual void updateForce(physx::PxVec3& force, double deltaTime) = 0;

    uint64_t getId() const;

protected:
    uint64_t _id; // Protected to allow derived classes access

private:
    static uint64_t _nextId; // Static counter for generating unique IDs
};