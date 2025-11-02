#include "ParticleSystem.h"

#include "ParticleGenerator.h"
#include "ParticleWithMass.h"
#include "ForceGenerator.h"
#include "ForceManager.h"


// Initialize static member
uint64_t ParticleSystem::_nextId = 0;

ParticleSystem::ParticleSystem()
    : _id(_nextId++)
    , _emitterOrigin(physx::PxVec3(0, 0, 0))
    , _forceManager(ForceManager::getInstance())
    , _registeredForceGenIds()
    , _isActive(true)
    , _isDead(false)

    , _forceGenActiveCount(0)
    , _maxInsideForceGenerators(Constants::System::MaxInsideForceGenerators)
    , _forceGensInsideSystem(_maxInsideForceGenerators)

    , _subSystemActiveCount(0)
    , _maxSubSystems(Constants::System::MaxSubSystems)
    , _subSystems(_maxSubSystems)
{
}

void ParticleSystem::setRenderable(bool renderable)
{
	_isRenderable = renderable;
}

void ParticleSystem::setDead()
{
	_isDead = true;
    setActive(false);
    
    for (auto& subSystem : _subSystems) {
        if (subSystem) {
            subSystem->setDead();
        }
    }

    deregisterAllInsideForceGens();
    deregisterAllForceGensAtForceManager();
}

bool ParticleSystem::mustKillParticle(const Particle &p, const ParticleGenerator &generator) const
{
    return generator.getLifetimePolicy().shouldDelete(generator.getDistribution(), p);
}

bool ParticleSystem::mustSpawnParticle(double deltaTime, const ParticleGenerator &generator) const
{
    return generator.getGenerationPolicy().shouldSpawn(generator.getDistribution(), deltaTime);
}

void ParticleSystem::applyForceManagerForces(ParticleWithMass &particle, const std::vector<ForceGenerator *> &forceGenerators)
{
    for (auto &forceGen : forceGenerators) {
        forceGen->applyForceOnParticle(particle);
    }
}

void ParticleSystem::applyInsideForces(ParticleWithMass &particle)
{
    for (int i = 0; i < _forceGenActiveCount; ++i) {
        _forceGensInsideSystem[i]->applyForceOnParticle(particle);
    }
}

void ParticleSystem::update(double deltaTime)
{
    if (_isActive) {
        updateInsideForces(deltaTime);
        updateSubSystems(deltaTime); 
    }
    else {
        std::cout << "ParticleSystem " << _id << " is inactive. Skipping update." << std::endl;
    }
}

void ParticleSystem::updateSubSystems(double deltaTime)
{
    for (int i = 0; i < _subSystemActiveCount; ++i) 
    {
        assert(_subSystems[i] != nullptr && "Sub-system inside particle system is null");

        if (_subSystems[i]->isActive()) {
            _subSystems[i]->update(deltaTime);
        }
        else if (_subSystems[i]->isDead()) {
            deregisterSubSystem(i);
            --i; // Adjust index after deregistration
        }
    }
}

void ParticleSystem::updateInsideForces(double deltaTime)
{
        for (int i = 0; i < _forceGenActiveCount; ++i) 
    {
        assert(_forceGensInsideSystem[i] != nullptr && "Force generator inside particle system is null");

        if (_forceGensInsideSystem[i]->isActive()) {
            _forceGensInsideSystem[i]->updateForce(deltaTime);
        }
        else if (_forceGensInsideSystem[i]->isDead()) {
            deregisterInsideForceGen(i);
            --i; // Adjust index after deregistration
        }
    }
}

void ParticleSystem::setActivateInsideForceGen(int forceGenIdx, bool active)
{
    assert(forceGenIdx >= 0 && forceGenIdx < _forceGenActiveCount && "Force generator index out of range");
    _forceGensInsideSystem[forceGenIdx]->setActive(active);
}

void ParticleSystem::setActivateForceGenAtForceManager(fGenId forceGenId, bool active)
{
    bool result = _forceManager.setActiveForceGenAtForceManager(forceGenId, active);
    if (!result) {
        std::cerr << "ForceGenerator with ID " << forceGenId << " not found in ForceManager." << std::endl;
    }
}

void ParticleSystem::registerSubSystem(std::unique_ptr<ParticleSystem> &subSystem)
{
    _subSystems.push_back(std::move(subSystem));
    _subSystemActiveCount++;
}

void ParticleSystem::registerInsideForceGen(std::unique_ptr<ForceGenerator> &forceGen)
{
    _forceGensInsideSystem.push_back(std::move(forceGen));
    _forceGenActiveCount++;
}

void ParticleSystem::registerForceGenAtForceManager(std::unique_ptr<ForceGenerator> &forceGen)
{
    _registeredForceGenIds.push_back(forceGen->getId());
    _forceManager.registerForceGenerator(_id, std::move(forceGen));
}

void ParticleSystem::deregisterSubSystem(int subSystemIdx)
{
    assert(subSystemIdx >= 0 && subSystemIdx < _subSystemActiveCount && "Sub-system index out of range");

    std::swap(_subSystems[subSystemIdx], _subSystems[_subSystemActiveCount - 1]);
    _subSystems[_subSystemActiveCount - 1] = nullptr; // Destroys the unique_ptr
    --_subSystemActiveCount;
}

void ParticleSystem::deregisterInsideForceGen(int forceGenIdx)
{
    assert(forceGenIdx >= 0 && forceGenIdx < _forceGenActiveCount && "Force generator index out of range");

    std::swap(_forceGensInsideSystem[forceGenIdx], _forceGensInsideSystem[_forceGenActiveCount - 1]);
    _forceGensInsideSystem[_forceGenActiveCount - 1] = nullptr; // Destroys the unique_ptr
    --_forceGenActiveCount;
}

void ParticleSystem::deregisterAllInsideForceGens()
{
    for (int i = 0; i < _forceGenActiveCount; ++i) {
        _forceGensInsideSystem[i] = nullptr; // Destroys the unique_ptr
    }
    _forceGenActiveCount = 0;
}

void ParticleSystem::deregisterForceGenAtForceManager(fGenId forceGenId)
{
    _forceManager.deregisterForceGenerator(_id, forceGenId);
}

void ParticleSystem::deregisterAllForceGensAtForceManager()
{
    for (const auto& forceGenId : _registeredForceGenIds) {
        _forceManager.deregisterForceGenerator(_id, forceGenId);
    }
    _registeredForceGenIds.clear();
}
