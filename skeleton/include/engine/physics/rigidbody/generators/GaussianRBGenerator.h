#pragma once

#include "RigidBodyGenerator.h"


class GaussianRigidBodyGenerator : public RigidBodyGenerator
{
protected:
	mutable std::normal_distribution<double> _n{ 0.0, 1.0 }; // mean 0.0, stddev 1.0

public:
	GaussianRigidBodyGenerator() = default;
	~GaussianRigidBodyGenerator() = default;

	double getDistribution() const override {
		return _n(_mt);
	}
};
