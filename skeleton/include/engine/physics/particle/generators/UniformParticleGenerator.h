#pragma once

#include "ParticleGenerator.h"


class UniformParticleGenerator : public ParticleGenerator
{
protected:

	mutable std::uniform_real_distribution<double> _u{ -1.0, 1.0 }; // defines the range of random values

public:

	UniformParticleGenerator() = default;
	~UniformParticleGenerator() = default;

	double getDistribution() const override {
		return _u(_mt);
	}

};
