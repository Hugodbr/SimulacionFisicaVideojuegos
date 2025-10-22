#include "ParticleGenerator.h"


ParticleGenerator::ParticleGenerator()
{
}

ParticleGenerator::~ParticleGenerator()
{
    delete _generationPolicy;
    delete _lifetimePolicy;
}

void ParticleGenerator::init(const physx::PxVec3& emitterOrigin, const Vector3Stats& velocity, const Particle* modelParticle)
{
    _emitterOrigin = emitterOrigin;
    _meanVelocity = velocity.mean;
    _velocityDeviation = velocity.deviation;
    _modelParticle = modelParticle;
}

void ParticleGenerator::setEmitterOrigin(const physx::PxVec3& emitterOrigin) {
    _emitterOrigin = emitterOrigin;
}

void ParticleGenerator::setMeanVelocity(const physx::PxVec3& meanVel) {
    _meanVelocity = meanVel;
}

void ParticleGenerator::setVelocityDeviation(const physx::PxVec3& velDeviation) {
    _velocityDeviation = velDeviation;
}

void ParticleGenerator::setModelParticle(const Particle* model) {
    _modelParticle = model;
}

void ParticleGenerator::setGenerationPolicy(const ParticleGenerationPolicy& genPolicy) {
    _generationPolicy = new ParticleGenerationPolicy(genPolicy);
}

void ParticleGenerator::setLifetimePolicy(const ParticleLifetimePolicy& lifePolicy) {
    _lifetimePolicy = new ParticleLifetimePolicy(lifePolicy);
}

physx::PxVec3 ParticleGenerator::getEmitterOrigin() const {
    return _emitterOrigin;
}

physx::PxVec3 ParticleGenerator::getMeanVelocity() const {
    return _meanVelocity;
}

physx::PxVec3 ParticleGenerator::getVelocityDeviation() const {
    return _velocityDeviation;
}

const Particle& ParticleGenerator::getModelParticle() const {
    return *_modelParticle;
}

ParticleGenerationPolicy& ParticleGenerator::getGenerationPolicy() const {
    return *_generationPolicy;
}

ParticleLifetimePolicy& ParticleGenerator::getLifetimePolicy() const {
    return *_lifetimePolicy;
}
