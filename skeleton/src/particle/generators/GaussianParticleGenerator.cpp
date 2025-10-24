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
