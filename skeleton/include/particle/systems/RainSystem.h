#pragma once

#include "ParticleSystem.h"
#include "ParticlePool.h"
#include "Region.h"

class UniformParticleGenerator;
class RainParticle;
class ForceGenerator;

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

	// Sets all active particles to renderable or not
	void setRenderable(bool renderable) override;

	// Returns the reserve count per generator for this system
	virtual uint64_t getReserveCountPerGenerator() const override { 
		return Constants::System::Rain::ReserveCountPerGenerator; 
	}

protected:

    void initParticleGeneratorAndPool();

	void createForceGenerator(std::unique_ptr<ForceGenerator>& forceGen);

	std::vector<
        std::pair<
            std::unique_ptr<UniformParticleGenerator>,
            std::unique_ptr<ParticlePool<RainParticle>>
            >
        > _generatorsAndPools;
};
