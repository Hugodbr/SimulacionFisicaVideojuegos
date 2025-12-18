#include "ParticleSystem.h"

#include "ParticleGenerator.h"
#include "Particle.h"


ParticleSystem::ParticleSystem()
    : ObjectSystem()
{
}

void ParticleSystem::setRenderableForParticle(Particle &particle)
{
    ObjectSystem::setRenderableForObject(particle);
}

bool ParticleSystem::mustKillParticle(const Particle &p, const ParticleGenerator &generator) const
{
    return ObjectSystem::mustKillObject(p, generator);
}

bool ParticleSystem::mustSpawnParticle(double deltaTime, const ParticleGenerator &generator) const
{
    return ObjectSystem::mustSpawnObject(deltaTime, generator);
}
