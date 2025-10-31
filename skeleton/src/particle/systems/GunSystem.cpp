#include "GunSystem.h"

// #include "Camera.h"
#include "UniformParticleGenerator.h"
#include "StaticParticle.h"


GunSystem::GunSystem(const physx::PxVec3 &position, Camera* cam)
    : ParticleSystem()
    , _generatorsAndPools()
    , _camera(cam)
{
    _emitterOrigin = position;
}

void GunSystem::init()
{
    initParticleGeneratorAndPool();
}

void GunSystem::initParticleGeneratorAndPool()
{
    float size = 0.9f;
    physx::PxVec4 color = Constants::Color::White;

    _generatorsAndPools.push_back({
        std::make_unique<UniformParticleGenerator>(),
        std::make_unique<ParticlePool<StaticParticle>>(
            getReserveCountPerGenerator(),  // Pool size
            size, // size particle
            color  // color particle
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

    ParticleGenerationPolicy::volumeShape meshShape;
	MeshData meshData; 
	meshData.loadMesh("../resources/flashLight.obj");
	new (&meshShape.mesh) MeshData(meshData);
	genPolicy.setRegion(SpawnRegionType::MESH, meshShape);
    generator->setGenerationPolicy(genPolicy);

    ParticleLifetimePolicy lifePolicy = ParticleLifetimePolicy();
	generator->setLifetimePolicy(lifePolicy);

    createGun();
}

void GunSystem::createGun()
{
    for (auto& [gen, pool] : _generatorsAndPools) 
	{
		int numToSpawn = Constants::System::Gun::ReserveCountPerGenerator;

		// Spawn "new" particles (getting from the pool)
		for (int i = 0; i < numToSpawn; ++i) 
		{
			auto* p = pool->activate();
			if (p) {
				physx::PxTransform t = physx::PxTransform(0, 0, 0, physx::PxQuat(0));
				t.p = gen->getGeneratedPosition();
                p->setTransformRelative(physx::PxTransform(_emitterOrigin - t.p, physx::PxQuat(0)));
				p->setTransform(t); 
				p->setAcceleration(physx::PxVec3(0.0f, 0.0f, 0.0f));
			}
			else break;
		}
	}
}

void GunSystem::update(double deltaTime)
{
    physx::PxTransform camTransform = _camera->getTransformRelativeToCamera(30.0f, 0.0f, 0.0f);
    

    for (auto& [gen, pool] : _generatorsAndPools) 
	{
		auto& particles = pool->accessParticlePool();
		for (int i = 0; i < pool->getActiveCount(); ++i) 
		{
            particles[i]->setTransform(
                _camera->getTransformRelativeToCamera(
                    30.0f, 0.0f, 0.0f
                )
            );
		}
	}
}

void GunSystem::setTransform(const physx::PxTransform &t)
{
    _emitterOrigin = t.p;
}
