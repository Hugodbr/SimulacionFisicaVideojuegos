#pragma once

#include "RigidBodyGenerator.h"


class UniformRigidBodyGenerator : public RigidBodyGenerator
{
protected:
	mutable std::uniform_real_distribution<double> _u{ -1.0, 1.0 }; // defines the range of random values

public:
	UniformRigidBodyGenerator() = default;
	~UniformRigidBodyGenerator() = default;

	double getDistribution() const override {
		return _u(_mt);
	}
};
