#include "ParticleGenerator.h"


ParticleGenerator::ParticleGenerator()
{
}

ParticleGenerator::~ParticleGenerator()
{
}

physx::PxVec3 ParticleGenerator::getOrigin() const
{
	return _origin;
}

physx::PxVec3 ParticleGenerator::getVelocity() const
{
	return _velocity;
}

double ParticleGenerator::getDuration() const 
{
	return _duration;
}

double ParticleGenerator::getGenerationProbability() const
{
	return _probability;
}

const Particle& ParticleGenerator::getModelParticle() const
{
	return *_modelParticle;
}
