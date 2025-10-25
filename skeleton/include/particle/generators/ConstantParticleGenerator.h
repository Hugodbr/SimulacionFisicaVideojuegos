#pragma once

#include "ParticleGenerator.h"


class ConstantParticleGenerator : public ParticleGenerator
{
public:

    ConstantParticleGenerator() = default;
    ~ConstantParticleGenerator() = default;

    std::list<std::unique_ptr<Particle>> generateParticles(double deltaTime) override;

    double getDistribution() const override;
};