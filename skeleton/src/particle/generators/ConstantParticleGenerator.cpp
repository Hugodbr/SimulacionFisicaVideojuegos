#include "ConstantParticleGenerator.h"


std::vector<std::unique_ptr<Particle>> ConstantParticleGenerator::generateParticles(double deltaTime)
{
    return std::vector<std::unique_ptr<Particle>>();
}

double ConstantParticleGenerator::getDistribution() const
{
    return 1.0;
}
