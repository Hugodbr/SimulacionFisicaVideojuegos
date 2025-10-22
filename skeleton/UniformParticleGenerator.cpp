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

std::list<std::unique_ptr<Particle>> UniformParticleGenerator::generateParticles(double deltaTime)
{
	std::list<std::unique_ptr<Particle>> generatedParticles;

	return generatedParticles;
}
