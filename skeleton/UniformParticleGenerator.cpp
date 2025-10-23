#include "UniformParticleGenerator.h"

#include "Particle.h"


UniformParticleGenerator::UniformParticleGenerator()
{
}

UniformParticleGenerator::~UniformParticleGenerator() 
{
}

double UniformParticleGenerator::getDistribution() const
{
	return _u(_mt);
}
