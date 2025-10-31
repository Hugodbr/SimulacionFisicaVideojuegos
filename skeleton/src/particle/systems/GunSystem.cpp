#include "GunSystem.h"

#include "MathUtils.h"
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
    float size = 0.3f;
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

    generator->init(_emitterOrigin);

    // Create generation policy
	ParticleGenerationPolicy genPolicy;
	_meshData.loadMeshFromFile("../resources/flashLight.obj");
	genPolicy.setRegion(Region(_meshData));
    generator->setGenerationPolicy(genPolicy);

    // Create lifetime policy
    ParticleLifetimePolicy lifePolicy;
	generator->setLifetimePolicy(lifePolicy);

    // Finally,
    createGun();
}

void GunSystem::createGun()
{
    for (auto& [gen, pool] : _generatorsAndPools) 
	{
		// int numToSpawn = Constants::System::Gun::ReserveCountPerGenerator;

		auto& vertices = _meshData.getMeshVertices();
        std::cout << "GunSystem::createGun -> vertices size: " << vertices.size() << std::endl;

		// Spawn "new" particles (getting from the pool)
        for (const auto& v : vertices)
		{
			auto* p = pool->activateParticle();
			if (p) {
				physx::PxTransform t = physx::PxTransform(v, physx::PxQuat(0));
				// t.p = gen->getGeneratedPosition();
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
    physx::PxTransform gunTransform = _camera->getTransformRelativeToCamera(
        _emitterOrigin.z, _emitterOrigin.x, _emitterOrigin.y
    );
    
    float stiffness = 10.0f; // how fast it catches up (try 5–15)
    float t = 1.0f - expf(-stiffness * (float)deltaTime); // framerate-independent smoothing

    for (auto& [gen, pool] : _generatorsAndPools) 
    {
        auto& particles = pool->accessParticlePool();
        for (int i = 0; i < pool->getActiveCount(); ++i) 
        {
            physx::PxTransform current = particles[i]->getTransform();

            // Target transform (camera-relative)
            physx::PxVec3 targetPos = gunTransform.transform(particles[i]->getRelativeTransform().p);
            physx::PxQuat targetRot = gunTransform.q;

            // Smoothly interpolate position and rotation
            physx::PxVec3 smoothedPos = lerp(current.p, targetPos, t);
            physx::PxQuat smoothedRot = slerp(current.q, targetRot, t);

            particles[i]->setTransform(physx::PxTransform(smoothedPos, smoothedRot));
        }
    }
}

void GunSystem::setTransform(const physx::PxTransform &t)
{
    _emitterOrigin = t.p;
}
