#include "GaussianParticleGenerator.h"


GaussianParticleGenerator::GaussianParticleGenerator()
{
}

GaussianParticleGenerator::~GaussianParticleGenerator()
{
}

double GaussianParticleGenerator::getDistribution() const
{
	return _n(_mt);
}

std::list<std::unique_ptr<Particle>> GaussianParticleGenerator::generateParticles(double deltaTime)
{
	return std::list<std::make_unique<Particle>()>;
}
