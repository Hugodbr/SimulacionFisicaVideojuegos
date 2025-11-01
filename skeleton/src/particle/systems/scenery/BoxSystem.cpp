#include "BoxSystem.h"

#include "ConstantParticleGenerator.h"
#include "StaticParticle.h"

BoxSystem::BoxSystem(const Region &region, float pointSize, double scale, const Vector4& color)
    : GridSystem(region, pointSize, scale, color)
{
    _meshData.loadMeshFromFile("../resources/floor.obj");
}

void BoxSystem::init()
{
    _emitterOrigin = physx::PxVec3(0, 0, 0);

    initParticleGeneratorAndPool();

    // Create particles in grid
    createParticlesInGrid(); 
}

void BoxSystem::update(double deltaTime)
{
}

void BoxSystem::initParticleGeneratorAndPool()
{
    _generatorsAndPools.push_back({
        std::make_unique<ConstantParticleGenerator>(),
        std::make_unique<ParticlePool<StaticParticle>>(getReserveCountPerGenerator(), 0.2f, Constants::Color::Green)
    });

    auto& generator = _generatorsAndPools[0].first;

    generator->init(_emitterOrigin);

    // Create generation policy
    ParticleGenerationPolicy genPolicy;
    genPolicy.setRegion(_region);
    generator->setGenerationPolicy(genPolicy);

    // Create lifetime policy
    ParticleLifetimePolicy lifePolicy;
    generator->setLifetimePolicy(lifePolicy);
}

void BoxSystem::createParticlesInGrid()
{
    auto& [generator, pool] = _generatorsAndPools[0];

    for (auto& vertex : _meshData.getMeshVertices()) 
    {
        Particle* p = pool->activateParticle();
        if (p) {
            // Set particle position to vertex position
            physx::PxVec3 pos = vertex * static_cast<float>(_scale) + _emitterOrigin;
            p->setTransform(physx::PxTransform(pos, physx::PxQuat(0)));
        }
    }

    std::cout << "BoxSystem::createParticlesInGrid -> Created " << pool->getActiveCount() << " particles." << std::endl;
}

