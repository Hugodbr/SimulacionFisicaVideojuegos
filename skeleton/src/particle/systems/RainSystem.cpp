#include "RainSystem.h"

#include "ForceManager.h"
#include "ForceGenerator.h"
#include "UniformParticleGenerator.h"
#include "Particle.h"
#include "RainParticle.h"
#include "MeshData.h"


RainSystem::RainSystem(const physx::PxVec3& origin, const Region& region)
	: ParticleSystem()
	, _generatorsAndPools()
	, _region(region)
	, _intensityLevel(IntensityLevel::LIGHT_RAIN)
{
	_emitterOrigin = origin;
}

void RainSystem::init()
{
	initParticleGeneratorAndPool();

}

void RainSystem::initParticleGeneratorAndPool()
{
	_generatorsAndPools.push_back({
        std::make_unique<UniformParticleGenerator>(),
        std::make_unique<ParticlePool<RainParticle>>(
			getReserveCountPerGenerator(),  // Pool size
			physx::PxTransform(0, 0, 0, physx::PxQuat(0)), // Initial transform particle
			physx::PxVec3(0, 0, 0) // Initial velocity particle
		)
    });

    auto& generator = _generatorsAndPools[0].first;

    generator->init(
        _emitterOrigin,
        Vector3Stats(physx::PxVec3(0, 0, 0), physx::PxVec3(0, 0, 0)) // velocity = 0
    );

    // Create generation policy
	ParticleGenerationPolicy genPolicy = ParticleGenerationPolicy(
		true, ScalarStats(10.0, 5.0),
		true, ScalarStats(0.0, 0.0)
	);

	physx::PxBounds3 box = physx::PxBounds3(
		physx::PxVec3(_region.shape.box.minimum.x, _region.shape.box.maximum.y - 1.0f, _region.shape.box.minimum.z),
		physx::PxVec3(_region.shape.box.maximum.x, _region.shape.box.maximum.y, _region.shape.box.maximum.z)
	);
	Region generationRegion(box);

	genPolicy.setRegion(generationRegion);

    generator->setGenerationPolicy(genPolicy);

    // Create lifetime policy
    ParticleLifetimePolicy lifePolicy = ParticleLifetimePolicy(_region, BoundType::SOLID);

    
	generator->setLifetimePolicy(lifePolicy);
}

void RainSystem::createForceGenerator(std::unique_ptr<ForceGenerator> &forceGen)
{
}

void RainSystem::update(double deltaTime)
{
	// Get all forces available from ForceManager
	std::vector<ForceGenerator*> forceGenerators = _forceManager.getForceGenerators();

	for (auto& [gen, pool] : _generatorsAndPools) 
	{
		int numToSpawn = 0;

		// Check if should spawn particles this frame
		if (mustSpawnParticle(deltaTime, *gen)) {
			numToSpawn = gen->getGenerationPolicy().spawnNumber(gen->getDistribution());
		}

		// Spawn "new" particles (getting from the pool)
		for (int i = 0; i < numToSpawn; ++i) 
		{
			auto* p = pool->activateParticle();
			if (p) {
				physx::PxTransform t = physx::PxTransform(0, 0, 0, physx::PxQuat(0));
				t.p = gen->getGeneratedPosition();
				p->setTransform(t);
				p->setAcceleration(physx::PxVec3(0.0f, 0.0f, 0.0f));
			}
			else break;
		}

		// Update active particles and deactivate if needed
		auto& particles = pool->accessParticlePool();
		for (int i = 0; i < pool->getActiveCount(); ++i) 
		{
			// std::cout << "Active Rain Particles: " << pool->getActiveCount() << std::endl;
			particles[i]->clearForces();
			// Apply all forces to the particle (from ForceManager and inside system)
			applyForceManagerForces(*particles[i], forceGenerators);
			applyInsideForces(*particles[i]);
			// Update particle
			particles[i]->update(deltaTime);

			if (mustKillParticle(*particles[i], *gen)) {
				pool->deactivate(i);
				--i; // Adjust index after deactivation
			}
		}
	}

}
