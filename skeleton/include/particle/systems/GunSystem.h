#pragma once

#include "ParticleSystem.h"
#include "ParticlePool.h"

class UniformParticleGenerator;
class StaticParticle;


class GunSystem : public ParticleSystem
{
private:
	physx::PxBounds3 _region;
	
public:

	GunSystem(const physx::PxVec3 &position, Camera* cam);
	~GunSystem() = default;

	void init() override;
	void update(double deltaTime) override;

    void setTransform(const physx::PxTransform& t);

	// Returns the reserve count per generator for this system
	virtual uint64_t getReserveCountPerGenerator() const override { 
		return Constants::System::Gun::ReserveCountPerGenerator; 
	}

protected:

    void initParticleGeneratorAndPool();
    void createGun();

    Camera* _camera;

	std::vector<
        std::pair<
            std::unique_ptr<UniformParticleGenerator>,
            std::unique_ptr<ParticlePool<StaticParticle>>
            >
        > _generatorsAndPools;
};