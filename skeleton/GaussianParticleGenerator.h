#pragma once
#include "ParticleGenerator.h"


class GaussianParticleGenerator : public ParticleGenerator
{
public:

	GaussianParticleGenerator();
	~GaussianParticleGenerator();

	std::list<Particle*> generateParticles(double deltaTime) override;

	// Setters
	//void setEmitterOrigin(const physx::PxVec3& pos) override;
	//
	//void setMeanVelocity(const physx::PxVec3& vel) override;
	//void setVelocityDeviation(const physx::PxVec3& dev) override;
	//
	//void setModelParticle(const Particle* model) override;

	//void setGenerationPolicy(const ParticleGenerationPolicy& genPolicy) override;
	//void setLifetimePolicy(const ParticleLifetimePolicy& lifePolicy) override;


protected:

	std::normal_distribution<double> _n{ 0.0, 1.0 };
};

