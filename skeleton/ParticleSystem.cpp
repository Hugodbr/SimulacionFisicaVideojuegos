#include "ParticleSystem.h"


ParticleSystem::ParticleSystem()
    : _modelParticle(nullptr)
    , _particleList()
    , _generatorList()
    , _emitterOrigin(physx::PxVec3(0, 0, 0))
{ }
