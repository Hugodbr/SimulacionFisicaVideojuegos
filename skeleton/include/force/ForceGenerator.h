#pragma once

#include <iostream>

#include <PxPhysicsAPI.h>

class ForceManager;


class ForceGenerator
{
public:
    ForceGenerator();
    virtual ~ForceGenerator() = default;

    virtual void updateForce(double deltaTime) = 0;

    virtual void setMagnitude(float magnitude);
    virtual void setForce(const physx::PxVec3& force);
    virtual void setForceDirection(const physx::PxVec3& direction);

    uint64_t getId() const { return _id; }

    float getMagnitude() const { return _magnitude; }
    physx::PxVec3 getForce() const { return _force; }
    physx::PxVec3 getForceDirection() const { return _force.getNormalized(); }

protected:
    uint64_t _id; // Protected to allow derived classes access

    physx::PxVec3 _force;
    physx::PxVec3 _direction;
    float _magnitude;

    ForceManager& _forceManager;

private:
    static uint64_t _nextId; // Static counter for generating unique IDs
};