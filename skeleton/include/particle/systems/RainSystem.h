#pragma once

#include "ParticleSystem.h"
#include "ParticlePool.h"
#include "Region.h"

class UniformParticleGenerator;
class RainParticle;

enum IntensityLevel {
	LIGHT_RAIN,
	MODERATE_RAIN,
	HEAVY_RAIN
};

class RainSystem : public ParticleSystem
{
private:
	Region _region;
	IntensityLevel _intensityLevel;
	
public:

	RainSystem(const physx::PxVec3 &origin, const Region& region);
	~RainSystem() = default;

	void init() override;
	void update(double deltaTime) override;

	// Returns the reserve count per generator for this system
	virtual uint64_t getReserveCountPerGenerator() const override { 
		return Constants::System::Rain::ReserveCountPerGenerator; 
	}

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
