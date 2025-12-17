// #pragma once

// #include "ParticleSystem.h"
// #include "ParticlePool.h"
// #include "Region.h"

// class GaussianParticleGenerator;
// class ForceGenerator;

// class TrashSystem : public ParticleSystem
// {
// private:
// 	Region _region;
	
// public:
// 	TrashSystem(const physx::PxVec3 &origin, const Region& region);
// 	~TrashSystem() = default;

// 	void init() override;
// 	void update(double deltaTime) override;
// 	virtual void render(const glm::mat4& modelViewMat) override;

// 	// Sets all active particles to renderable or not
// 	void setRenderable(bool renderable) override;

// 	// Returns the reserve count per generator for this system
// 	virtual uint64_t getReserveCountPerGenerator() const override { 
// 		return Constants::System::Trash::ReserveCountPerGenerator; 
// 	}

// protected:
//     void initGenerator();

// protected:

// 	std::unique_ptr<GaussianParticleGenerator> _generator;
// };
