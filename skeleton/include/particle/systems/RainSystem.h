#pragma once

#include "ParticleSystem.h"


class RainSystem : public ParticleSystem
{
public:

	RainSystem(const physx::PxVec3 &origin);
	~RainSystem() = default;

	void init() override;
	void update(double deltaTime) override;


protected:

	void createParticleGenerator() override;
};
