#include "ParticleGenerator.h"


// Must initialize
ParticleGenerator::ParticleGenerator()
    : _mt(std::random_device{}())
    , _generationPolicy(std::make_unique<ParticleGenerationPolicy>()) // Must initialize
    , _lifetimePolicy(std::make_unique<ParticleLifetimePolicy>()) // Must initialize
{ }

void ParticleGenerator::init(
    const physx::PxVec3& emitterOrigin, 
    const Vector3Stats& velocity
)
{
    _emitterOrigin = emitterOrigin;
    _meanVelocity = velocity.mean;
    _velocityDeviation = velocity.deviation;// hide the model particle
}

void ParticleGenerator::setEmitterOrigin(const physx::PxVec3& emitterOrigin) {
    _emitterOrigin = emitterOrigin;
    _generationPolicy->setEmitterOrigin(emitterOrigin);
}

void ParticleGenerator::setMeanVelocity(const physx::PxVec3& meanVel) {
    _meanVelocity = meanVel;
}

void ParticleGenerator::setVelocityDeviation(const physx::PxVec3& velDeviation) {
    _velocityDeviation = velDeviation;
}
void ParticleGenerator::setGenerationPolicy(const ParticleGenerationPolicy& genPolicy) {
    _generationPolicy = std::make_unique<ParticleGenerationPolicy>(genPolicy);
    _generationPolicy->setEmitterOrigin(_emitterOrigin);
    _generationPolicy->setVelocity(Vector3Stats(_meanVelocity, _velocityDeviation));
}

void ParticleGenerator::setLifetimePolicy(const ParticleLifetimePolicy& lifePolicy) {
    _lifetimePolicy = std::make_unique<ParticleLifetimePolicy>(lifePolicy);
}

int ParticleGenerator::numberOfGenerations() {
    return _generationPolicy->spawnNumber(getDistribution());
}

physx::PxVec3 ParticleGenerator::getGeneratedPosition()
{
    return _generationPolicy->generatePosition([this]() {
            return getDistribution();
            });
}

physx::PxVec3 ParticleGenerator::getGeneratedVelocity()
{
    return _generationPolicy->generateVelocity([this]() {
            return getDistribution();
            });
}

physx::PxVec4 ParticleGenerator::getGeneratedColor()
{
    return _generationPolicy->generateColor([this]() {
            return getDistribution();
            });
}
