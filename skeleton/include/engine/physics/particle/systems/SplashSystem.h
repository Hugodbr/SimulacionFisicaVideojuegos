#pragma once

#include "ParticleSystem.h"
#include "ParticlePool.h"
#include "Region.h"

class GaussianParticleGenerator;
class RainParticle;
class ForceGenerator;

class SplashSystem : public ParticleSystem
{
private:
	Region _region;
	
public:
	SplashSystem(const physx::PxVec3 &origin, const Region& region);
	~SplashSystem() = default;

	void init() override;
	void update(double deltaTime) override;
	virtual void onRender(const glm::mat4& modelViewMat) override;

    virtual void setEmitterOrigin(const physx::PxVec3& origin) override;
	virtual void setCustomGenerationTriggerCallback(std::function<bool()> callback) override;

	// Sets all active particles to renderable or not
	void setRenderable(bool renderable) override;

	virtual void setEmissionRate(int spawnMeanRate) override;
	virtual int getEmissionRate() const override;

	// Returns the reserve count per generator for this system
	virtual uint64_t getReserveCountPerGenerator() const override { 
		return Constants::System::Rain::ReserveCountPerGenerator; 
	}

protected:
    void initParticleGeneratorAndPool();
	
	// Clear all forces from the particles and apply forces from ForceManager
	virtual void applyForces() override;

protected:

	std::vector<
        std::pair<
            std::unique_ptr<GaussianParticleGenerator>,
            std::unique_ptr<ParticlePool<RainParticle>>
            >
        > _generatorsAndPools;
};
