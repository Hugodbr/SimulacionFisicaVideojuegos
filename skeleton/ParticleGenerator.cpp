#include "ParticleGenerator.h"

#include "Particle.h"


ParticleGenerator::ParticleGenerator()
{
}

ParticleGenerator::~ParticleGenerator()
{
    delete _generationPolicy;
    delete _lifetimePolicy;
}

void ParticleGenerator::init(const physx::PxVec3& emitterOrigin, const Vector3Stats& velocity, const Particle& modelParticle)
{
    _emitterOrigin = emitterOrigin;
    _meanVelocity = velocity.mean;
    _velocityDeviation = velocity.deviation;
    _modelParticle = &modelParticle;
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

void ParticleGenerator::setModelParticle(const Particle& model) { // TODO VER ISSO DAS REFERENCIAS
    _modelParticle = &model;
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

std::list<std::unique_ptr<Particle>> ParticleGenerator::generateParticles(double deltaTime) 
{
    if (_generationPolicy->useSpawnInterval) {
        if (!_generationPolicy->shouldSpawn(getDistribution(), deltaTime)) { // if NOT should spawn return an empty list
            return std::list<std::unique_ptr<Particle>>();
        }
    }

    // Create particles
    std::list<std::unique_ptr<Particle>> generatedParticles;
    int numOfGenerations = _generationPolicy->spawnNumber(getDistribution());

    for (int i = 0; i < numOfGenerations; ++i)
    {
        std::unique_ptr<Particle> newParticle = std::move(_modelParticle.clone());
        // Setup particle
        physx::PxTransform t = physx::PxTransform();
        t.p = _generationPolicy->generatePosition(getDistribution());
        newParticle->setOrigin(t);

        generatedParticles.push_back(std::move(newParticle));
    }

    return generatedParticles;
}
