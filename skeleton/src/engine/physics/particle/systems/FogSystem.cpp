#include "FogSystem.h"

#include "ForceManager.h"
#include "ForceGenerator.h"
#include "UniformParticleGenerator.h"
#include "Particle.h"
#include "RainParticle.h"
#include "MeshData.h"
#include "HurricaneForce.h"
#include "ExplosionForce.h"


FogSystem::FogSystem(const physx::PxVec3& origin, const Region& region)
	: ParticleSystem()
	, _generatorsAndPools()
	, _region(region)
{
	_emitterOrigin = origin;
}

void FogSystem::init()
{
	initParticleGeneratorAndPool();
	createForceGenerator();
}

void FogSystem::initParticleGeneratorAndPool()
{
	_generatorsAndPools.push_back({
        std::make_unique<UniformParticleGenerator>(),
        std::make_unique<ParticlePool<RainParticle>>(
			getReserveCountPerGenerator(),  // Pool size
			physx::PxTransform(0, 0, 0, physx::PxQuat(0)) // Initial transform particle
		)
    });

    auto& generator = _generatorsAndPools[0].first;

    generator->init(
        _emitterOrigin,
        Vector3Stats(physx::PxVec3(0, 0, 0), physx::PxVec3(10, 10, 10)) // velocity = 0
    );

    // Create generation policy
	ParticleGenerationPolicy genPolicy = ParticleGenerationPolicy(
		true, ScalarStats(50.0, 5.0),
		true, ScalarStats(0.0, 0.0)
	);

	physx::PxBounds3 box = physx::PxBounds3(
		physx::PxVec3(_region.shape.box.minimum.x, _region.shape.box.minimum.y, _region.shape.box.minimum.z),
		physx::PxVec3(_region.shape.box.maximum.x, _region.shape.box.maximum.y, _region.shape.box.maximum.z)
	);
	Region generationRegion(box);

	genPolicy.setRegion(generationRegion);

    generator->setGenerationPolicy(genPolicy);

    // Create lifetime policy
    ParticleLifetimePolicy lifePolicy = ParticleLifetimePolicy(_region, BoundType::SOLID);

	generator->setLifetimePolicy(lifePolicy);
}

void FogSystem::createForceGenerator()
{

}

void FogSystem::applyForces()
{
	// Get all forces available from ForceManager
	std::vector<ForceGenerator*> forceGenerators = _forceManager.getForceGenerators();

	for (auto& [generator, pool] : _generatorsAndPools) // for each pool
	{
		for (int i = 0; i < pool->getActiveCount(); ++i) // for each active particle in the pool
		{
			auto& particle = pool->accessParticlePool()[i];
			particle->clearForces();

			for (auto& forceGen : forceGenerators) // for each force generator
			{
				if (forceGen->isActive() && doForceAffectsSystem(*forceGen)) {
					particle->applyForce(*forceGen);
				}
			}
		}
	}
}

void FogSystem::update(double deltaTime)
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

void FogSystem::render(const glm::mat4 &modelViewMat)
{
	assert(_renderableEntity && "Renderable entity not set for FogSystem.");

    auto& [generator, pool] = _generatorsAndPools[0];

    for (auto& particle : pool->accessParticlePool()) 
    {
        if (particle->isActive()) {
            _renderableEntity->setWPos(
                particle->getTransform().p.x,
                particle->getTransform().p.y,
                particle->getTransform().p.z
            );
            _renderableEntity->render(modelViewMat);
        }
    }
}

void FogSystem::setRenderable(bool renderable)
{
	ParticleSystem::setRenderable(renderable);

	std::cout << "FogSystem setRenderable: " << renderable << std::endl;

	// Update visibility of particles
	for (auto& [generator, pool] : _generatorsAndPools)
	{
		for (int i = 0; i < pool->getActiveCount(); ++i)
		{
			setRenderableForParticle(*pool->accessParticlePool()[i]);
		}
	}
}
