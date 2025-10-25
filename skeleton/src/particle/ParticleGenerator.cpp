#include "ParticleGenerator.h"



// Must initialize
ParticleGenerator::ParticleGenerator()
    : _mt(std::random_device{}())
    , _generationPolicy(std::make_unique<ParticleGenerationPolicy>()) // Must initialize
    , _lifetimePolicy(std::make_unique<ParticleLifetimePolicy>()) // Must initialize
{ }

void ParticleGenerator::init(
    const Particle& modelParticle,
    const physx::PxVec3& emitterOrigin, 
    const Vector3Stats& velocity
)
{
    _emitterOrigin = emitterOrigin;
    _meanVelocity = velocity.mean;
    _velocityDeviation = velocity.deviation;
    _modelParticle = modelParticle.clone();

    _modelParticle->setVisibility(false); // hide the model particle
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

void ParticleGenerator::setModelParticle(const Particle& model) {
    _modelParticle = model.clone();
}

void ParticleGenerator::setGenerationPolicy(const ParticleGenerationPolicy& genPolicy) {
    _generationPolicy = std::make_unique<ParticleGenerationPolicy>(genPolicy);
}

void ParticleGenerator::setLifetimePolicy(const ParticleLifetimePolicy& lifePolicy) {
    _lifetimePolicy = std::make_unique<ParticleLifetimePolicy>(lifePolicy);
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
    //std::cout << "ParticleGenerator -> generate particles." << std::endl; OK

    if (_generationPolicy->useSpawnInterval) {
        if (!_generationPolicy->shouldSpawn(getDistribution(), deltaTime)) { // if NOT should spawn return an empty list
            return std::list<std::unique_ptr<Particle>>();
        }
    }

    //std::cout << "ParticleGenerator -> generate particles -> before creating particles." << std::endl;
    // Create particles
    std::list<std::unique_ptr<Particle>> generatedParticles;
    const int numOfGenerations = _generationPolicy->spawnNumber(getDistribution());

    for (int i = 0; i < numOfGenerations; ++i)
    {
        //std::cout << "ParticleGenerator -> generate particles -> generating particles." << std::endl;

        std::unique_ptr<Particle> newParticle = _modelParticle->clone();
        // Setup particle
        physx::PxTransform t = physx::PxTransform();
        t.p = _generationPolicy->generatePosition([this]() {
            return getDistribution();
            });
        newParticle->setOrigin(t);

        generatedParticles.push_back(std::move(newParticle));
    }

    return generatedParticles;
}
