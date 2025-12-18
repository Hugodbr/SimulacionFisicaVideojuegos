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

bool ParticleSystem::doForceAffectsSystem(const ForceGenerator &forceGen) const
{
    for (const auto &group : _groups) {
        if (group == Constants::Group::DynamicGroup::NONE) {
            return false;
        }
        if (forceGen.getGroup() == group 
        || forceGen.getGroup() == Constants::Group::DynamicGroup::ALL 
        || (forceGen.getParticleSystem() != nullptr && forceGen.getParticleSystem()->getId() == this->getId())) {
            return true;
        }
    }
    return false;
}
