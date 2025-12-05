#pragma once

#include "ParticleGenerator.h"


class ConstantParticleGenerator : public ParticleGenerator
{
public:

    ConstantParticleGenerator() = default;
    ~ConstantParticleGenerator() = default;

    double getDistribution() const override {
        return 0.0;
    }
    
};