#pragma once

#include "ObjectGenerator.h"

class GaussianGenerator : public ObjectGenerator
{
protected:
	mutable std::normal_distribution<double> _n{ 0.0, 1.0 }; // mean 0.0, stddev 1.0

public:
	GaussianGenerator() = default;
	~GaussianGenerator() = default;

	double getDistribution() const override {
		return _n(_mt);
	}
};
