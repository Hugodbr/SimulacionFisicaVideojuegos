#include "MeshSystem.h"

#include "ParticleGenerator.h"
#include "ConstantParticleGenerator.h"
#include "ParticlePool.h"
#include "ParticleWithMass.h"


MeshSystem::MeshSystem(const std::string &filename)
    : _pointSize(3.0f)
    , _scale(1.0)
    , _color(Constants::Color::White)
{
    _meshData.loadMeshFromFile(filename);
}

MeshSystem::MeshSystem(const std::string &filename, float pointSize, double scale, const Vector4& color)
    : _pointSize(pointSize)
    , _scale(scale)
    , _color(color)
{
    _meshData.loadMeshFromFile(filename);
}

void MeshSystem::init()
{
    initParticleGeneratorAndPool();
}

void MeshSystem::update(double deltaTime)
{
    for (auto& [generator, pool] : _generatorsAndPools) 
    {
        for (int i = 0; i < pool->getActiveCount(); ++i) 
        {
            pool->accessParticlePool()[i]->update(deltaTime);
        }
    }
}

void MeshSystem::initParticleGeneratorAndPool()
{
    _generatorsAndPools.push_back({
        std::make_unique<ConstantParticleGenerator>(),
        std::make_unique<ParticlePool<ParticleWithMass>>(getReserveCountPerGenerator()
        , 2.0f // mass
        , _pointSize // size
        , Constants::Color::Green)
    });

    auto& generator = _generatorsAndPools[0].first;

    generator->init(_emitterOrigin);

    // Create generation policy
	ParticleGenerationPolicy genPolicy;
    generator->setGenerationPolicy(genPolicy);

    // Create lifetime policy
    ParticleLifetimePolicy lifePolicy;
    generator->setLifetimePolicy(lifePolicy);
    // Finally,
    createParticlesAtMeshVertices();
}

void MeshSystem::createParticlesAtMeshVertices()
{
    auto& [generator, pool] = _generatorsAndPools[0];

    for (auto& vertex : _meshData.getMeshVertices()) 
    {
        Particle* p = pool->activateParticle();
        if (p) {
            // Set particle position to vertex position
            physx::PxVec3 pos = vertex * _scale + _emitterOrigin;
            p->setTransform(physx::PxTransform(pos, physx::PxQuat(0)));
            p->setVelocity(physx::PxVec3(5.0f, 0, 0));
        }
    }
}