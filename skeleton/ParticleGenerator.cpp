#include "ParticleGenerator.h"


ParticleGenerator::ParticleGenerator()
{
}

ParticleGenerator::~ParticleGenerator()
{
}

physx::PxVec3 ParticleGenerator::getMeanPosition() const {
    return _meanPosition;
}

physx::PxVec3 ParticleGenerator::getMeanVelocity() const {
    return _meanVelocity;
}

double ParticleGenerator::getMeanLifetime() const {
    return _meanLifetime;
}

physx::PxVec3 ParticleGenerator::getPositionDeviation() const {
    return _positionDeviation;
}

physx::PxVec3 ParticleGenerator::getVelocityDeviation() const {
    return _velocityDeviation;
}

double ParticleGenerator::getLifetimeDeviation() const {
    return _lifetimeDeviation;
}

double ParticleGenerator::getSpawnProbability() const {
    return _spawnProbability;
}

int ParticleGenerator::getSpawnCount() const {
    return _spawnCount;
}

const Particle& ParticleGenerator::getModelParticle() const {
    return *_modelParticle;
}
