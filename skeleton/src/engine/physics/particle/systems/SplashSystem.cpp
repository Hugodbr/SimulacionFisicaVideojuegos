#include "SplashSystem.h"

#include "ForceManager.h"
#include "ForceGenerator.h"
#include "UniformParticleGenerator.h"
#include "GaussianParticleGenerator.h"
#include "Particle.h"
#include "RainParticle.h"

#include "ParticleGenerationPolicy.h"
#include "ParticleLifetimePolicy.h"


SplashSystem::SplashSystem(const physx::PxVec3& origin, const Region& region)
	: ParticleSystem()
	, _generatorsAndPools()
	, _region(region)
{
	_emitterOrigin = origin;
}

void SplashSystem::init()
{
	initParticleGeneratorAndPool();
	createForceGenerator();
}

void SplashSystem::initParticleGeneratorAndPool()
{
	_generatorsAndPools.push_back({
        std::make_unique<GaussianParticleGenerator>(),
        std::make_unique<ParticlePool<RainParticle>>(
			Constants::System::Splash::ReserveCountPerGenerator,  // Pool size
			physx::PxTransform(0, 0, 0, physx::PxQuat(0)) // Initial transform particle
		)
    });

    auto& generator = _generatorsAndPools[0].first;

    generator->init(
        _emitterOrigin,
        Vector3Stats(physx::PxVec3(0.5, 3.0f, -4.0f), physx::PxVec3(0.2, 2.0f, 1.0f)) // velocity = 0
    );

    // Create generation policy
	ParticleGenerationPolicy genPolicy = ParticleGenerationPolicy(
		true, ScalarStats(60.0, 30.0),
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
    ParticleLifetimePolicy lifePolicy = ParticleLifetimePolicy(ScalarStats(0.7f, 0.15f));

	generator->setLifetimePolicy(lifePolicy);
}

void SplashSystem::createForceGenerator()
{

}

void SplashSystem::applyForces()
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

void SplashSystem::update(double deltaTime)
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
				physx::PxVec3 vel = gen->getGeneratedVelocity();
				p->setVelocity(vel);
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
				pool->deactivateParticle(i);
				--i; // Adjust index after deactivation
			}
		}
	}

}

void SplashSystem::onRender(const glm::mat4 &modelViewMat)
{
	assert(_renderableEntity && "Renderable entity not set for SplashSystem.");

    auto& [generator, pool] = _generatorsAndPools[0];

    for (auto& particle : pool->accessParticlePool()) 
    {
        if (particle->isActive()) {
			_renderableEntity->setPose(
				glm::vec3(
					particle->getPosition().x,
					particle->getPosition().y,
					particle->getPosition().z
				),
				glm::quat(
					particle->getTransform().q.w,
					particle->getTransform().q.x,
					particle->getTransform().q.y,
					particle->getTransform().q.z
				)
			);
            _renderableEntity->render(modelViewMat);
        }
    }
}

void SplashSystem::setEmitterOrigin(const physx::PxVec3 &origin)
{
    _emitterOrigin = origin;

    for (auto& [generator, pool] : _generatorsAndPools) 
    {
        generator->setEmitterOrigin(_emitterOrigin);
    }
}

void SplashSystem::setCustomGenerationTriggerCallback(std::function<bool()> callback)
{
	auto& generator = _generatorsAndPools[0].first;

	generator->getGenerationPolicy().setCustomCallback(callback);
}

void SplashSystem::setRenderable(bool renderable)
{
	ParticleSystem::setRenderable(renderable);

	std::cout << "SplashSystem setRenderable: " << renderable << std::endl;

	// Update visibility of particles
	for (auto& [generator, pool] : _generatorsAndPools)
	{
		for (int i = 0; i < pool->getActiveCount(); ++i)
		{
			setRenderableForParticle(*pool->accessParticlePool()[i]);
		}
	}
}
