#pragma once

#include "ParticleSystem.h"


class RainSystem : public ParticleSystem
{
public:

	RainSystem(const physx::PxVec3 &origin);
	~RainSystem() = default;

	void init() override;
	void update(double deltaTime) override;

	// Returns the reserve count per generator for this system
	virtual uint64_t getReserveCountPerGenerator() const override;
protected:

	void createParticleGenerators() override;
};
