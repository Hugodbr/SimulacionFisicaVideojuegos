#pragma once
#include "ParticleGenerator.h"

template<typename T>
class UniformParticleGenerator : public ParticleGenerator<T>
{
public:

	UniformParticleGenerator();
	~UniformParticleGenerator();

	std::list<std::unique_ptr<Particle>> generateParticles(double deltaTime) override;

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
