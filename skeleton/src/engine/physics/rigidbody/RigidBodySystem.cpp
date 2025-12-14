#include "RigidBodySystem.h"

#include "ParticleGenerator.h"
#include "ParticleWithMass.h"
#include "ForceGenerator.h"
#include "ForceManager.h"


// Initialize static member
uint64_t RigidBodySystem::_nextId = 0;

RigidBodySystem::RigidBodySystem()
    : _id(_nextId++)
    // , _emitterOrigin(physx::PxVec3(0, 0, 0))
    , _forceManager(ForceManager::getInstance())
    , _registeredForceGenIds()
    , _isActive(true)
    , _isDead(false)
    , _isRenderable(true)
    // , _worldRegion(Region(physx::PxBounds3(
    //     physx::PxVec3(-1000.0f, -1000.0f, -1000.0f),
    //     physx::PxVec3(1000.0f, 1000.0f, 1000.0f)
    // )))
    // , _subSystemActiveCount(0)
    // , _maxSubSystems(Constants::System::MaxSubSystems)
{
    // _subSystems.reserve(_maxSubSystems);
}

void RigidBodySystem::setRenderable(bool renderable)
{
	_isRenderable = renderable;
}

// void RigidBodySystem::setRenderableForParticle(Particle &particle)
// {
//     if (_isRenderable && !particle.isVisible()) {
//         particle.setVisibility(true);
//     }
//     else if (!_isRenderable) {
//         particle.setVisibility(false);
//     }
// }

void RigidBodySystem::setDead()
{
	_isDead = true;
    setActive(false);
    
    // for (auto& subSystem : _subSystems) {
    //     if (subSystem) {
    //         subSystem->setDead();
    //     }
    // }

    deregisterAllForceGensAtForceManager();
}

bool RigidBodySystem::doForceAffectsSystem(const ForceGenerator &forceGen) const
{
    for (const auto &group : _groups) {
        if (group == Constants::Group::DynamicGroup::NONE) {
            return false;
        }
        if (forceGen.getGroup() == group 
        || forceGen.getGroup() == Constants::Group::DynamicGroup::ALL 
        || (forceGen.getRigidBodySystem() != nullptr && forceGen.getRigidBodySystem()->getId() == this->getId())) {
            return true;
        }
    }
    return false;
}

// bool RigidBodySystem::mustKillParticle(const Particle &p, const ParticleGenerator &generator) const
// {
//     return generator.getLifetimePolicy().shouldDelete(generator.getDistribution(), p);
// }

// bool RigidBodySystem::mustSpawnParticle(double deltaTime, const ParticleGenerator &generator) const
// {
//     return generator.getGenerationPolicy().shouldSpawn(generator.getDistribution(), deltaTime);
// }

void RigidBodySystem::load()
{
    _renderableEntity->load();
}

void RigidBodySystem::unload()
{
    _renderableEntity->unload();
}

void RigidBodySystem::update(double deltaTime)
{
    if (_isActive) {
        // updateSubSystems(deltaTime); 
        for (auto& rb : _rigidbodies) {
            rb->update(deltaTime);
        }
    }
    else {
        std::cout << "RigidBodySystem " << _id << " is inactive. Skipping update." << std::endl;
    }
}

// void RigidBodySystem::setWorldRegion(const Region &region)
// {
// 	_worldRegion = Region(region);
// }

void RigidBodySystem::setRenderableEntity(std::shared_ptr<Abs_Entity> renderable)
{
    _renderableEntity = renderable;
}

void RigidBodySystem::render(const glm::mat4 &modelViewMat)
{
    for (auto& rb : _rigidbodies) {
        rb->updateRenderableEntityPose(); // because the renderable is shared among all rigid bodies in the system
        _renderableEntity->render(modelViewMat);
    }
}

// void RigidBodySystem::updateSubSystems(double deltaTime)
// {
//     for (int i = 0; i < _subSystemActiveCount; ++i) 
//     {
//         assert(_subSystems[i] != nullptr && "Sub-system inside particle system is null");

//         if (_subSystems[i]->isActive()) {
//             _subSystems[i]->update(deltaTime);
//         }
//         else if (_subSystems[i]->isDead()) {
//             deregisterSubSystem(i);
//             --i; // Adjust index after deregistration
//         }
//     }
// }

void RigidBodySystem::setActivateForceGenAtForceManager(fGenId forceGenId, bool active)
{
    bool result = _forceManager.setActiveForceGenAtForceManager(forceGenId, active);
    if (!result) {
        std::cerr << "ForceGenerator with ID " << forceGenId << " not found in ForceManager." << std::endl;
    }
}

// void RigidBodySystem::registerSubSystem(std::unique_ptr<RigidBodySystem> subSystem)
// {
//     _subSystems.push_back(std::move(subSystem));
//     _subSystemActiveCount++;
// }

void RigidBodySystem::registerForceGenAtForceManager(std::unique_ptr<ForceGenerator> forceGen)
{
    _registeredForceGenIds.push_back(forceGen->getId());
    _forceManager.registerForceGenerator(_id, std::move(forceGen));
}

// void RigidBodySystem::deregisterSubSystem(int subSystemIdx)
// {
//     assert(subSystemIdx >= 0 && subSystemIdx < _subSystemActiveCount && "Sub-system index out of range");

//     std::swap(_subSystems[subSystemIdx], _subSystems[_subSystemActiveCount - 1]);
//     _subSystems[_subSystemActiveCount - 1] = nullptr; // Destroys the unique_ptr
//     --_subSystemActiveCount;
// }

void RigidBodySystem::deregisterForceGenAtForceManager(fGenId forceGenId)
{
    _forceManager.deregisterForceGenerator(_id, forceGenId);
}

void RigidBodySystem::deregisterAllForceGensAtForceManager()
{
    for (const auto& forceGenId : _registeredForceGenIds) {
        _forceManager.deregisterForceGenerator(_id, forceGenId);
    }
    _registeredForceGenIds.clear();
}
