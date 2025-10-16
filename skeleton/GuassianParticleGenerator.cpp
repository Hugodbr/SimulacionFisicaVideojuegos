#include "GuassianParticleGenerator.h"

GuassianParticleGenerator::GuassianParticleGenerator()
{
}

GuassianParticleGenerator::~GuassianParticleGenerator()
{
}

void GuassianParticleGenerator::init(const Vector3Stats& position, const Vector3Stats& velocity, const ScalarStats lifetime, const Particle* modelParticle)
{
    _meanPosition = position.mean;
    _positionDeviation = position.deviation;
    _meanVelocity = velocity.mean;
    _velocityDeviation = velocity.deviation;
    _meanLifetime = lifetime.mean;
    _lifetimeDeviation = lifetime.deviation;
    _modelParticle = modelParticle;
}

void GuassianParticleGenerator::setMeanPosition(const physx::PxVec3& pos) {
    _meanPosition = pos;
}

void GuassianParticleGenerator::setPositionDeviation(const physx::PxVec3& dev) {
    _positionDeviation = dev;
}

void GuassianParticleGenerator::setMeanVelocity(const physx::PxVec3& vel) {
    _meanVelocity = vel;
}

void GuassianParticleGenerator::setVelocityDeviation(const physx::PxVec3& dev) {
    _velocityDeviation = dev;
}

void GuassianParticleGenerator::setMeanLifetime(double mean) {
    _meanLifetime = mean;
}

void GuassianParticleGenerator::setLifetimeDeviation(double dev) {
    _lifetimeDeviation = dev;
}

void GuassianParticleGenerator::setSpawnProbability(double probability) {
    _spawnProbability = probability;
}

void GuassianParticleGenerator::setSpawnCount(int count) {
    _spawnCount = count;
}

void GuassianParticleGenerator::setModelParticle(const Particle* model)
{
    _modelParticle = model;
}

std::list<Particle*> GuassianParticleGenerator::generateParticles()
{
	return std::list<Particle*>();
}
