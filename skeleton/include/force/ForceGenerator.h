// #pragma once

// #include <iostream>

// #include <PxPhysicsAPI.h>

// #include "ForceManager.h"


// class ForceGenerator
// {
// public:
//     ForceGenerator();
//     virtual ~ForceGenerator() = default;

//     virtual void updateForce(physx::PxVec3& force, double deltaTime) = 0;

//     virtual void setMagnitude(float magnitude);
//     virtual void setForce(const physx::PxVec3& force);
//     virtual void setForceDirection(const physx::PxVec3& direction);

//     uint64_t getId() const;

//     float getMagnitude() const;
//     physx::PxVec3 getForce() const;
//     physx::PxVec3 getForceDirection() const;

// protected:
//     uint64_t _id; // Protected to allow derived classes access

//     physx::PxVec3 _force;
//     float _magnitude;

// private:
//     static uint64_t _nextId; // Static counter for generating unique IDs
// };