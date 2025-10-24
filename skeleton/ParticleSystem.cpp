#include "ParticleSystem.h"

#include "UniformParticleGenerator.h"


ParticleSystem::ParticleSystem()
    : _emitterOrigin(physx::PxVec3(0, 0, 0))
{
    _generatorAndChildParticlesList = {};
}

bool ParticleSystem::mustDelete(const Particle& p, const ParticleGenerator& gen) const
{
    return false;
}