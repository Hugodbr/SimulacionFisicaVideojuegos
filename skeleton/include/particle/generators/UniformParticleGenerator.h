#pragma once
#include "ParticleGenerator.h"


class UniformParticleGenerator : public ParticleGenerator
{
public:

	UniformParticleGenerator();
	~UniformParticleGenerator();

	double getDistribution() const override;

	// Setters
	//void setEmitterOrigin(const physx::PxVec3& pos) override;

	//void setMeanVelocity(const physx::PxVec3& vel) override;
	//void setVelocityDeviation(const physx::PxVec3& dev) override;

	//void setModelParticle(const Particle* model) override;

	//void setGenerationPolicy(const ParticleGenerationPolicy& policy) override;
	//void setLifetimePolicy(const ParticleLifetimePolicy& policy) override;


protected:

	mutable std::uniform_real_distribution<double> _u{ -1.0, 1.0 }; // defines the range of random values
};
