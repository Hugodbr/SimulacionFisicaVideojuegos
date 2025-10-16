#pragma once
#include "ParticleGenerator.h"


class GuassianParticleGenerator : public ParticleGenerator
{
public:
	GuassianParticleGenerator();
	~GuassianParticleGenerator();

	void init(
		const Vector3Stats& position,
		const Vector3Stats& velocity,
		const ScalarStats lifetime,
		const Particle* modelParticle
	) override;

	// Setters
	void setMeanPosition(const physx::PxVec3& pos) override;
	void setPositionDeviation(const physx::PxVec3& dev) override;
	
	void setMeanVelocity(const physx::PxVec3& vel) override;
	void setVelocityDeviation(const physx::PxVec3& dev) override;
	
	void setMeanLifetime(double mean) override;
	void setLifetimeDeviation(double dev) override;
	
	void setSpawnProbability(double probability) override;
	void setSpawnCount(int count) override;
	
	void setModelParticle(const Particle* model) override;


	std::list<Particle*> generateParticles() override;

protected:

	std::normal_distribution<double> _n{ 0.0, 1.0 };
};

