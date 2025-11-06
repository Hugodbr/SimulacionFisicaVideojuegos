#include "RainSystem.h"

#include "ForceManager.h"
#include "ForceGenerator.h"
#include "UniformParticleGenerator.h"
#include "Particle.h"
#include "RainParticle.h"
#include "MeshData.h"
#include "HurricaneForce.h"
#include "ExplosionForce.h"


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
	createForceGenerator();
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
		true, ScalarStats(50.0, 5.0),
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

void RainSystem::createForceGenerator()
{
	Region hurricaneRegion(_region);

	// Center of the hurricane = rain region center
	physx::PxVec3 eye = physx::PxVec3(
		(_region.shape.box.minimum.x + _region.shape.box.maximum.x) / 2.0f,
		(_region.shape.box.minimum.y + _region.shape.box.maximum.y) / 2.0f,
		(_region.shape.box.minimum.z + _region.shape.box.maximum.z) / 2.0f
	);

	std::unique_ptr<ForceGenerator> forceGen = std::make_unique<HurricaneForce>(
		this,
		hurricaneRegion,
		eye,
		physx::PxVec3(0.0f, 10.0f, 0.0f)
	);
	forceGen->setGroup(Constants::Group::DynamicGroup::ENVIRONMENT);
	forceGen->setTimer(6.0); // Delay activation by 6 seconds
	forceGen->setExpireTime(10.0); // Expire after 10 seconds
	forceGen->setActive(false); // Initially inactive

	std::cout << "RainSystem::createForceGenerator -> HurricaneForce created." << std::endl;

	registerForceGenAtForceManager(std::move(forceGen));

	// Explosion
	std::unique_ptr<ForceGenerator> explosionForceGen = std::make_unique<ExplosionForce>(
		this,
		eye,
		100000.0f,   // max radius
		1000000.0f, // magnitude
		300000.0f // expansion velocity
	);

	std::cout << "RainSystem::createForceGenerator -> ExplosionForce created." << std::endl;

	explosionForceGen->setGroup(Constants::Group::DynamicGroup::ENVIRONMENT);
	explosionForceGen->setTimer(15.0); // Delay activation by 15 seconds
	explosionForceGen->setExpireTime(20.0); // Expire after 15 seconds
	explosionForceGen->setActive(false); // Initially inactive
	registerForceGenAtForceManager(std::move(explosionForceGen));

}

void RainSystem::applyForces()
{
	// Get all forces available from ForceManager
	std::vector<ForceGenerator*> forceGenerators = _forceManager.getForceGenerators();

	for (auto& [generator, pool] : _generatorsAndPools) // for each pool
	{
		for (auto& forceGen : forceGenerators) // for each force generator
		{
			if (forceGen->isActive() && doForceAffectsSystem(*forceGen)) {

				for (int i = 0; i < pool->getActiveCount(); ++i) 
				{
					auto& particle = pool->accessParticlePool()[i];
					particle->clearForces();
					particle->applyForce(*forceGen);
				}
			}
		}
	}
}

void RainSystem::update(double deltaTime)
{
	// ! IMPORTANT ! Must call base update to handle sub-systems and inside forces
	ParticleSystem::update(deltaTime);
	if (!isActive()) {
		return;
	}

	// Apply forces to all particles
	applyForces();

	// Update each generator and its pool
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
				setRenderableForParticle(*p);
				physx::PxTransform t = physx::PxTransform(0, 0, 0, physx::PxQuat(0));
				t.p = gen->getGeneratedPosition();
				p->setTransform(t);
			}
			else break;
		}

		// Update active particles and deactivate if needed
		auto& particles = pool->accessParticlePool();
		for (int i = 0; i < pool->getActiveCount(); ++i) 
		{
			// std::cout << "Active Rain Particles: " << pool->getActiveCount() << std::endl;
			// Update particle
			particles[i]->update(deltaTime);

			if (mustKillParticle(*particles[i], *gen)) {
				pool->deactivate(i);
				--i; // Adjust index after deactivation
			}
		}
	}

}

void RainSystem::setRenderable(bool renderable)
{
	ParticleSystem::setRenderable(renderable);

	std::cout << "RainSystem setRenderable: " << renderable << std::endl;

	// Update visibility of particles
	for (auto& [generator, pool] : _generatorsAndPools)
	{
		for (int i = 0; i < pool->getActiveCount(); ++i)
		{
			setRenderableForParticle(*pool->accessParticlePool()[i]);
		}
	}
}
