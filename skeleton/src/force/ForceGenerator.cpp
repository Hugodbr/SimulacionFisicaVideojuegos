// #include "ForceGenerator.h"


// // Initialize static member
// uint64_t ForceGenerator::_nextId = 0;

// ForceGenerator::ForceGenerator()
//     : _id(_nextId++)
// {
// }

// void ForceGenerator::setMagnitude(float magnitude) {
//     _magnitude = magnitude;
// }

// void ForceGenerator::setForce(const physx::PxVec3 &force) {
//     _force = force;
// }

// void ForceGenerator::setForceDirection(const physx::PxVec3 &direction) {
//     physx::PxVec3 dirNormalized = direction.getNormalized();
//     _force = dirNormalized * _magnitude;
// }

// uint64_t ForceGenerator::getId() const
// { 
//     return _id; 
// }

// float ForceGenerator::getMagnitude() const {
//     return _magnitude;
// }

// physx::PxVec3 ForceGenerator::getForce() const {
//     return _force;
// }

// physx::PxVec3 ForceGenerator::getForceDirection() const {
//     return _force.getNormalized();
// }