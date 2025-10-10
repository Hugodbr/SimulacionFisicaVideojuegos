#include "UniformParticleGenerator.h"


UniformParticleGenerator::UniformParticleGenerator()
{
}

UniformParticleGenerator::~UniformParticleGenerator()
{
}

void UniformParticleGenerator::init(const physx::PxVec3& origin, const physx::PxVec3& velocity, double duration, const Particle& modelParticle)
{
	setOrigin(origin);
	setVelocity(velocity);
	setDuration(duration);
	setModelParticle(modelParticle);
}


void UniformParticleGenerator::setOrigin(const physx::PxVec3& origin)
{
	_origin = origin;
}

void UniformParticleGenerator::setVelocity(const physx::PxVec3& velocity)
{
	_velocity = velocity;
}

void UniformParticleGenerator::setDuration(double duration)
{
	_duration = duration;
}

void UniformParticleGenerator::setGenerationProbability(double probability)
{
	_probability = probability;
}

void UniformParticleGenerator::setModelParticle(const Particle& modelParticle)
{
	_modelParticle = _modelParticle;
}

std::list<Particle*> UniformParticleGenerator::generateParticles()
{
	return std::list<Particle*>();
}
