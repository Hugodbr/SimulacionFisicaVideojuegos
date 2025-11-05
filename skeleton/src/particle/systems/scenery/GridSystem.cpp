#include "GridSystem.h"

#include "ConstantParticleGenerator.h"
#include "StaticParticle.h"
#include "ForceGenerator.h"


GridSystem::GridSystem(const Region &region, float pointSize, double scale, const Vector4& color)
    : ParticleSystem()
    , _region(region)
    , _pointSize(pointSize)
    , _scale(scale)
    , _color(color)
    , _visible(true)
{
    _group = { Constants::Group::DynamicGroup::NONE };
}

void GridSystem::init()
{
    _emitterOrigin = physx::PxVec3(0, 0, 0);

    initParticleGeneratorAndPool();

    // Create particles in grid
    createParticlesInGrid(); 
}

void GridSystem::update(double deltaTime)
{
    // ! IMPORTANT ! Must call base update to handle sub-systems and inside forces
	ParticleSystem::update(deltaTime);
	if (!isActive()) {
		return;
	}
}

void GridSystem::setRenderable(bool renderable)
{
    ParticleSystem::setRenderable(renderable);

    // Update visibility of particles
    for (auto& [generator, pool] : _generatorsAndPools) 
    {
        for (int i = 0; i < pool->getActiveCount(); ++i) 
        {
            pool->accessParticlePool()[i]->setVisibility(renderable);
        }
    }
}

void GridSystem::applyForces()
{
    // Stable - No forces applied
}

void GridSystem::initParticleGeneratorAndPool()
{
    _generatorsAndPools.push_back({
        std::make_unique<ConstantParticleGenerator>(),
        std::make_unique<ParticlePool<StaticParticle>>(getReserveCountPerGenerator(), 0.2f, Constants::Color::Green)
    });

    auto& generator = _generatorsAndPools[0].first;

    generator->init(_emitterOrigin);

    // Create generation policy
	ParticleGenerationPolicy genPolicy;
    generator->setGenerationPolicy(genPolicy);

    // Create lifetime policy
    ParticleLifetimePolicy lifePolicy;
    generator->setLifetimePolicy(lifePolicy);
}

void GridSystem::createParticlesInGrid()
{
    auto& particlePool = _generatorsAndPools.begin()->second;
    
    // Particle at origin
    auto particle = particlePool->activateParticle();
    physx::PxTransform particleTransform(physx::PxVec3(0, 0, 0), physx::PxQuat(0));

    if (particle) {
        particle->setTransform(particleTransform);
        particle->setColor(Constants::Color::Red);
        particle->setSize(10.0f);
    }

    float a_minX = _region.shape.box.minimum.x;
    float a_maxX = _region.shape.box.maximum.x;
    float a_minY = _region.shape.box.minimum.y;
    float a_maxY = _region.shape.box.maximum.y;
    float a_minZ = _region.shape.box.minimum.z;
    float a_maxZ = _region.shape.box.maximum.z;

    for (float x = a_minX; x <= a_maxX; x += _scale) 
    {
        for (float y = a_minY; y <= a_maxY; y += _scale) 
        {
            for (float z = a_minZ; z <= a_maxZ; z += _scale) 
            {
                physx::PxTransform particleTransform(physx::PxVec3(x, y, z));

                for (auto& _generatorsAndPools_it = _generatorsAndPools.begin(); _generatorsAndPools_it != _generatorsAndPools.end(); ++_generatorsAndPools_it)
                {
                    auto* particle = _generatorsAndPools_it->second->activateParticle();
                    if (particle) {
                        particle->setTransform(particleTransform);
                        particle->setColor(Vector4(
                            (x/a_maxX) * 255.0f,
                            255.0f,
                            0.0f,
                            255.0f
                        ));
                    }
                }
            }
        }
    }
}
