#include "RainSystem.h"

#include "ForceManager.h"
#include "UniformParticleGenerator.h"
#include "Particle.h"
#include "RainParticle.h"
#include "MeshData.h"


RainSystem::RainSystem(const physx::PxVec3& origin, const physx::PxBounds3& region)
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

	// Create the region shape for the policy
	// ParticleGenerationPolicy::volumeShape boxShape;
	// new (&boxShape.box) physx::PxBounds3(physx::PxVec3(-100.0f, -100.0f, -100.0f), physx::PxVec3(100.0f, 100.0f, 100.0f));
	// ParticleGenerationPolicy::volumeShape sphereShape;
	// new (&sphereShape.sphere) Vector3Stats(physx::PxVec3(0.0f, 0.0f, 0.0f), physx::PxVec3(10.0f, 10.0f, 10.0f));
	// ParticleGenerationPolicy::volumeShape pointShape;
	// new (&pointShape.point) Vector3Stats();

	// Set the region of generation: region type and shape
	// genPolicy.setRegion(SpawnRegionType::BOX, boxShape);
	// genPolicy.setRegion(SpawnRegionType::SPHERE, sphereShape);
	// genPolicy.setRegion(SpawnRegionType::POINT, pointShape);

	// ParticleGenerationPolicy::volumeShape meshShape;
	// MeshData meshData; 
	// // meshData.loadMesh("../resources/cone.obj");
	// meshData.loadMesh("../resources/monkey.obj");
	// new (&meshShape.mesh) MeshData(meshData);
	// genPolicy.setRegion(SpawnRegionType::MESH, meshShape);

	ParticleGenerationPolicy::volumeShape boxShape;
	new (&boxShape.box) physx::PxBounds3(
		physx::PxVec3(
			_region.minimum.x,
			_region.maximum.y - 1.0f,
			_region.minimum.z
		),
		physx::PxVec3(
			_region.maximum.x,
			_region.maximum.y,
			_region.maximum.z
		)
	);
	genPolicy.setRegion(SpawnRegionType::BOX, boxShape);

    generator->setGenerationPolicy(genPolicy);

    // Create lifetime policy
	// Vector3Stats sphere = Vector3Stats(physx::PxVec3(0, 0, 0), physx::PxVec3(50, 50, 50));
	// ParticleLifetimePolicy::volumeShape boxShape;
	// new (&boxShape.box) physx::PxBounds3(_region);
    ParticleLifetimePolicy lifePolicy = ParticleLifetimePolicy(_region, BoundType::SOLID);

	// lifePolicy.setRegion(SpawnRegionType::BOX, boxShape);
    
	generator->setLifetimePolicy(lifePolicy);
}

void RainSystem::update(double deltaTime)
{
	physx::PxVec3 globalForce = _forceManager.getGlobalResultingForce();

	for (auto& [gen, pool] : _generatorsAndPools) 
	{
		int numToSpawn = 0;

		// Check if should spawn particles this frame
		if (gen->getGenerationPolicy().shouldSpawn(gen->getDistribution(), deltaTime)) {
			numToSpawn = gen->getGenerationPolicy().spawnNumber(gen->getDistribution());
		}

		// Spawn "new" particles (getting from the pool)
		for (int i = 0; i < numToSpawn; ++i) 
		{
			auto* p = pool->activate();
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
			particles[i]->applyForce(globalForce);
			particles[i]->applyForce(_forceManager.applyGlobalForceOnParticle(*particles[i], deltaTime));
			particles[i]->applyRegisteredForces();
			particles[i]->update(deltaTime);

			if (gen->getLifetimePolicy().shouldDelete(gen->getDistribution(), *particles[i])) {
				pool->deactivate(i);
				--i; // Adjust index after deactivation
			}
		}
	}

}
