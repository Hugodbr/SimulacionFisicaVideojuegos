#include "GridSystem.h"

#include "ConstantParticleGenerator.h"
#include "StaticParticle.h"


GridSystem::GridSystem(const physx::PxBounds3 &region, float pointSize, double scale, const Vector4& color)
    : ParticleSystem()
    , _region(region)
    , _pointSize(pointSize)
    , _scale(scale)
    , _color(color)
{
}

void GridSystem::init()
{
    _emitterOrigin = physx::PxVec3(0, 0, 0);

    _modelParticle = std::make_unique<Particle>(
        physx::PxTransform(_emitterOrigin), 
        physx::PxVec3(0, 0, 0),
        physx::PxVec3(0, 0, 0), 
        Constants::Integration_Method::NONE, 
        _pointSize, 1.0, _color
    );

    // std::vector<std::pair<ConstantParticleGenerator, ParticlePool<StaticParticle>>> _generatorsAndPools;


    // Hide the model particle
    _modelParticle->setVisibility(false);

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

    // for (auto& genAndPool : _generatorsAndPools) 
    // {
    //     for (int i = 0; i < genAndPool.second->getActiveCount(); ++i) 
    //     {
    //         genAndPool.second->particles()[i]->setVisibility(_visible);
    //     }
    // }
}

uint64_t GridSystem::getReserveCountPerGenerator() const
{
    return Constants::System::Grid::ReserveCountPerGenerator;
}

void GridSystem::initParticleGeneratorAndPool()
{
    _generatorsAndPools.push_back({
        std::make_unique<ConstantParticleGenerator>(),
        std::make_unique<ParticlePool<StaticParticle>>(getReserveCountPerGenerator(), 0.2f, Constants::Color::Green)
    });

    auto& generator = _generatorsAndPools[0].first;

    generator->init(
        *_modelParticle,
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

// ConstantParticleGenerator GridSystem::createParticleGenerator()
// {
//     // Create the generator
// 	ConstantParticleGenerator generator = ConstantParticleGenerator();

//     generator.init(
//         *_modelParticle,
//         _emitterOrigin,
//         Vector3Stats(physx::PxVec3(0, 0, 0), physx::PxVec3(0, 0, 0)) // velocity = 0
//     );

//     // Create generation policy
// 	ParticleGenerationPolicy genPolicy = ParticleGenerationPolicy();
//     generator.setGenerationPolicy(genPolicy);

//     // Create lifetime policy
//     ParticleLifetimePolicy lifePolicy = ParticleLifetimePolicy();
//     generator.setLifetimePolicy(lifePolicy);

//     return generator;
// }

// void GridSystem::initParticlePools()
// {
//     // Create particle pool for StaticParticle
//     ParticlePool<StaticParticle> particlePool(getReserveCountPerGenerator());

//     // Store generator and pool pair
//     _generatorsAndPools.emplace_back(
//         std::make_pair(
//             ConstantParticleGenerator(), // Placeholder, actual generator should be created and initialized
//             std::move(particlePool)
//         )
//     );
// }

void GridSystem::createParticlesInGrid()
{
    // Particle at origin
    physx::PxTransform particleTransform(physx::PxVec3(0, 0, 0));
    // std::unique_ptr<Particle> originParticle = _modelParticle->clone();
    // originParticle->setOrigin(particleTransform);
    // originParticle->setColor(Constants::Color::Red);

    auto a = _generatorsAndPools.begin()->second->particles();

    for (auto& _generatorsAndPools_it = _generatorsAndPools.begin(); _generatorsAndPools_it != _generatorsAndPools.end(); ++_generatorsAndPools_it)
    {
        auto* particle = _generatorsAndPools_it->second->activate();
        if (particle) {
            particle->setOrigin(particleTransform);
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
                // std::unique_ptr<Particle> newParticle = _modelParticle->clone();
                // newParticle->setOrigin(particleTransform);
                // newParticle->setColor(Constants::Color::White);

                for (auto& _generatorsAndPools_it = _generatorsAndPools.begin(); _generatorsAndPools_it != _generatorsAndPools.end(); ++_generatorsAndPools_it)
                {
                    auto* particle = _generatorsAndPools_it->second->activate();
                    if (particle) {
                        particle->setOrigin(particleTransform);
                        particle->setColor(Constants::Color::Green);
                    }
                }
            }
        }
    }
}
