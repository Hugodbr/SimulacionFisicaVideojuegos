#pragma once

#include "ParticleGenerator.h"


class GaussianParticleGenerator : public ParticleGenerator
{
protected:

	mutable std::normal_distribution<double> _n{ 0.0, 1.0 }; // mean 0.0, stddev 1.0

public:

	GaussianParticleGenerator() = default;
	~GaussianParticleGenerator() = default;

	double getDistribution() const override {
		return _n(_mt);
	}

};
