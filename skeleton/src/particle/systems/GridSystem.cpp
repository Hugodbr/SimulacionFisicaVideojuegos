#include "GridSystem.h"

#include "ConstantParticleGenerator.h"
#include "StaticParticle.h"


GridSystem::GridSystem(const physx::PxBounds3 &region, float pointSize, double scale, const Vector4& color)
    : ParticleSystem()
    , _region(region)
    , _pointSize(pointSize)
    , _scale(scale)
    , _color(color)
    , _visible(true)
{
}

void GridSystem::init()
{
    _emitterOrigin = physx::PxVec3(0, 0, 0);

    // std::vector<std::pair<ConstantParticleGenerator, ParticlePool<StaticParticle>>> _generatorsAndPools;

    // // Create the particle generator
    // ConstantParticleGenerator generator = createParticleGenerator();

    initParticleGeneratorAndPool();

    // Create particles in grid
    createParticlesInGrid(); 
}

void GridSystem::update(double deltaTime)
{
}

void GridSystem::toggleVisibility()
{
    _visible = !_visible;

    for (auto& [generator, pool] : _generatorsAndPools) 
    {
        for (int i = 0; i < pool->getActiveCount(); ++i) 
        {
            pool->accessParticlePool()[i]->setVisibility(_visible);
        }
    }
}

void GridSystem::initParticleGeneratorAndPool()
{
    _generatorsAndPools.push_back({
        std::make_unique<ConstantParticleGenerator>(),
        std::make_unique<ParticlePool<StaticParticle>>(getReserveCountPerGenerator(), 0.2f, Constants::Color::Green)
    });

    auto& generator = _generatorsAndPools[0].first;

    generator->init(
        _emitterOrigin,
        Vector3Stats(physx::PxVec3(0, 0, 0), physx::PxVec3(0, 0, 0)) // velocity = 0
    );

    // Create generation policy
	ParticleGenerationPolicy genPolicy = ParticleGenerationPolicy();
    generator->setGenerationPolicy(genPolicy);

    // Create lifetime policy
    ParticleLifetimePolicy lifePolicy = ParticleLifetimePolicy();
    generator->setLifetimePolicy(lifePolicy);
}

void GridSystem::createParticlesInGrid()
{
    // Particle at origin
    physx::PxTransform particleTransform(physx::PxVec3(0, 0, 0));
    // std::unique_ptr<Particle> originParticle = _modelParticle->clone();
    // originParticle->setOrigin(particleTransform);
    // originParticle->setColor(Constants::Color::Red);

    auto a = _generatorsAndPools.begin()->second->accessParticlePool();

    for (auto& _generatorsAndPools_it = _generatorsAndPools.begin(); _generatorsAndPools_it != _generatorsAndPools.end(); ++_generatorsAndPools_it)
    {
        auto* particle = _generatorsAndPools_it->second->activate();
        if (particle) {
            particle->setTransform(particleTransform);
            particle->setColor(Constants::Color::Red);
        }
    }

    // // Assuming only one generator in the list
    // _generatorAndChildParticlesList.begin()->second.push_back(
    //     std::move(originParticle)
    // );

    float a_minX = _region.minimum.x;
    float a_maxX = _region.maximum.x;
    float a_minY = _region.minimum.y;
    float a_maxY = _region.maximum.y;
    float a_minZ = _region.minimum.z;
    float a_maxZ = _region.maximum.z;

    for (float x = a_minX; x <= a_maxX; x += _scale) 
    {
        for (float y = a_minY; y <= a_maxY; y += _scale) 
        {
            for (float z = a_minZ; z <= a_maxZ; z += _scale) 
            {
                physx::PxTransform particleTransform(physx::PxVec3(x, y, z));

                for (auto& _generatorsAndPools_it = _generatorsAndPools.begin(); _generatorsAndPools_it != _generatorsAndPools.end(); ++_generatorsAndPools_it)
                {
                    auto* particle = _generatorsAndPools_it->second->activate();
                    if (particle) {
                        particle->setTransform(particleTransform);
                        particle->setColor(Constants::Color::Green);
                    }
                }
            }
        }
    }
}
