#pragma once

#include "ObjectGenerator.h"

class UniformGenerator : public ObjectGenerator
{
protected:
	mutable std::uniform_real_distribution<double> _u{ -1.0, 1.0 }; // defines the range of random values

public:
	UniformGenerator() = default;
	~UniformGenerator() = default;

	double getDistribution() const override {
		return _u(_mt);
	}
};
