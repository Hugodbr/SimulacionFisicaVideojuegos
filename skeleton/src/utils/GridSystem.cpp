#include "GridSystem.h"

#include "Policies.h"
#include "ConstantParticleGenerator.h"


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

    // Hide the model particle
    _modelParticle->setVisibility(false);

    // Create the particle generator
    createParticleGenerator();

    // Create particles in grid
    createParticlesInGrid(); 
}

void GridSystem::update(double deltaTime)
{
}

void GridSystem::toggleVisibility(bool visible)
{
    for (auto& generatorAndParticles : _generatorAndChildParticlesList)
    {
        for (auto& particle : generatorAndParticles.second)
        {
            particle->setVisibility(visible);
        }
    }
}

void GridSystem::createParticleGenerator()
{
    // Create the generator
	std::unique_ptr<ParticleGenerator> generator = std::make_unique<ConstantParticleGenerator>();

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

    // Add generator to the list
    _generatorAndChildParticlesList.emplace_back(
        std::move(generator), 
        std::list<std::unique_ptr<Particle>>()
    );
}

void GridSystem::createParticlesInGrid()
{
    // Particle at origin
    physx::PxTransform particleTransform(physx::PxVec3(0, 0, 0));
    std::unique_ptr<Particle> originParticle = _modelParticle->clone();
    originParticle->setOrigin(particleTransform);
    originParticle->setColor(Constants::Color::Red);
    
    // Assuming only one generator in the list
    _generatorAndChildParticlesList.begin()->second.push_back(
        std::move(originParticle)
    );

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
                std::unique_ptr<Particle> newParticle = _modelParticle->clone();
                newParticle->setOrigin(particleTransform);
                newParticle->setColor(Constants::Color::White);

                // Assuming only one generator in the list
                _generatorAndChildParticlesList.begin()->second.push_back(std::move(newParticle));
            }
        }
    }
}
