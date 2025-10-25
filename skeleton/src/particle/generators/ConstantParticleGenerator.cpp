#include "ConstantParticleGenerator.h"


std::list<std::unique_ptr<Particle>> ConstantParticleGenerator::generateParticles(double deltaTime)
{
    return std::list<std::unique_ptr<Particle>>();
}

double ConstantParticleGenerator::getDistribution() const
{
    return 1.0;
}
