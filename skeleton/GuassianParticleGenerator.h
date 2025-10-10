#pragma once
#include "ParticleGenerator.h"


class GuassianParticleGenerator : public ParticleGenerator
{
public:
	GuassianParticleGenerator();
	~GuassianParticleGenerator();

	void init() override;

	// Setters
	void setOrigin(const physx::PxVec3& origin) override;
	void setVelocity(const physx::PxVec3& velocity) override;
	void setDuration(double duration) override;
	void setGenerationProbability(double probability) override;

	std::list<Particle*> generateParticles() override;

protected:

	std::normal_distribution<double> _n{ 0, 1 };
};

