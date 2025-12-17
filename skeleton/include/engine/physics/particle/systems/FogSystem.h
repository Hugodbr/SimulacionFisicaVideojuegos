#pragma once

#include "ParticleSystem.h"
#include "ParticlePool.h"
#include "Region.h"

class UniformParticleGenerator;
class RainParticle;
class ForceGenerator;

class FogSystem : public ParticleSystem
{
private:
	Region _region;
	
public:
	FogSystem(const physx::PxVec3 &origin, const Region& region);
	~FogSystem() = default;

	void init() override;
	void update(double deltaTime) override;
	virtual void render(const glm::mat4& modelViewMat) override;

	// Sets all active particles to renderable or not
	void setRenderable(bool renderable) override;

	// Returns the reserve count per generator for this system
	virtual uint64_t getReserveCountPerGenerator() const override { 
		return Constants::System::Rain::ReserveCountPerGenerator; 
	}

protected:
    void initParticleGeneratorAndPool();

	// void createForceGenerator(std::unique_ptr<ForceGenerator>& forceGen);
	void createForceGenerator();
	
	// Clear all forces from the particles and apply forces from ForceManager
	virtual void applyForces() override;

protected:

	std::vector<
        std::pair<
            std::unique_ptr<UniformParticleGenerator>,
            std::unique_ptr<ParticlePool<RainParticle>>
            >
        > _generatorsAndPools;
};
