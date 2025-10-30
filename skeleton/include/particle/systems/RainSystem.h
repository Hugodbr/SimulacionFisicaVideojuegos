#pragma once

#include "ParticleSystem.h"
#include "ParticlePool.h"

class UniformParticleGenerator;
class RainParticle;

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

    void initParticleGeneratorAndPool();


	// void createParticleGenerators() override;

	    std::vector<
        std::pair<
            std::unique_ptr<UniformParticleGenerator>,
            std::unique_ptr<ParticlePool<RainParticle>>
            >
        > _generatorsAndPools;
};
