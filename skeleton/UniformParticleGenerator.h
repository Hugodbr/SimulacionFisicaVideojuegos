#pragma once
#include "ParticleGenerator.h"


class UniformParticleGenerator : public ParticleGenerator
{
public:

	UniformParticleGenerator();
	~UniformParticleGenerator();

	void init(
		const physx::PxVec3& origin,
		const physx::PxVec3& velocity,
		double duration,
		const Particle& modelParticle
	) override;

	// Setters
	void setOrigin(const physx::PxVec3& origin) override;
	void setVelocity(const physx::PxVec3& velocity) override;
	void setDuration(double duration) override;
	void setGenerationProbability(double probability) override;
	void setModelParticle(const Particle& modelParticle) override;

	std::list<Particle*> generateParticles() override;

protected:

	std::uniform_real_distribution<double> _u{ 0, 1 };
};

