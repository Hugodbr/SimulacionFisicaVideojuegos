#include "ForceGenerator.h"

#include "Constants.h"
#include "ParticleSystem.h"

// Initialize static member
uint64_t ForceGenerator::_nextId = 0;

ForceGenerator::ForceGenerator()
    : _id(_nextId++), _active(true), _dead(false), _magnitude(0.0f), _force(physx::PxVec3(0.0f, 0.0f, 0.0f)), _particleSystem(nullptr)
{
    std::cout << "Creating ForceGenerator with no associated ParticleSystem! ID: " << _id << std::endl;
}

ForceGenerator::ForceGenerator(const ParticleSystem *particleSystem)
    : _id(_nextId++), _active(true), _dead(false), _magnitude(0.0f), _force(physx::PxVec3(0.0f, 0.0f, 0.0f))
{
    initParticleSysForceGen(const_cast<ParticleSystem*>(particleSystem));
}
