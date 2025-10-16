#include "UniformParticleGenerator.h"

#include "Particle.h"


UniformParticleGenerator::UniformParticleGenerator() 
{
}

UniformParticleGenerator::~UniformParticleGenerator() 
{
}

void UniformParticleGenerator::init(
    const Vector3Stats& position,
    const Vector3Stats& velocity,
    const ScalarStats lifetime,
    const Particle* modelParticle
) {
    _meanPosition = position.mean;
    _positionDeviation = position.deviation;
    _meanVelocity = velocity.mean;
    _velocityDeviation = velocity.deviation;
    _meanLifetime = lifetime.mean;
    _lifetimeDeviation = lifetime.deviation;
    _modelParticle = modelParticle;
}

void UniformParticleGenerator::setMeanPosition(const physx::PxVec3& pos) {
    _meanPosition = pos;
}

void UniformParticleGenerator::setPositionDeviation(const physx::PxVec3& dev) {
    _positionDeviation = dev;
}

void UniformParticleGenerator::setMeanVelocity(const physx::PxVec3& vel) {
    _meanVelocity = vel;
}

void UniformParticleGenerator::setVelocityDeviation(const physx::PxVec3& dev) {
    _velocityDeviation = dev;
}

void UniformParticleGenerator::setMeanLifetime(double mean) {
    _meanLifetime = mean;
}

void UniformParticleGenerator::setLifetimeDeviation(double dev) {
    _lifetimeDeviation = dev;
}

void UniformParticleGenerator::setSpawnProbability(double probability) {
    _spawnProbability = probability;
}

void UniformParticleGenerator::setSpawnCount(int count) {
    _spawnCount = count;
}

void UniformParticleGenerator::setModelParticle(const Particle* model) {
    _modelParticle = model;
}

std::list<Particle*> UniformParticleGenerator::generateParticles()
{
	std::list<Particle*> generatedParticles;

	int generationVolume = _volume + _u(_mt) * _volume;

	for (int i = 0; i < generationVolume; ++i) {
		Particle* newParticle = _modelParticle->clone();
		
		newParticle->setLifeTime(_duration * _u(_mt));

		physx::PxTransform originTransform = physx::PxTransform(
			_origin.x + _u(_mt),
			_origin.y + _u(_mt),
			_origin.z + _u(_mt)
		);
		newParticle->setOrigin(originTransform);
		newParticle->setVelocity(physx::PxVec3(.0f, .0f, .0f));

		generatedParticles.push_back(newParticle);
	}

	return generatedParticles;
}
