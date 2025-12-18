#include "RigidBodySystem.h"

#include "ParticleGenerator.h"
#include "ParticleWithMass.h"
#include "ForceGenerator.h"
#include "ForceManager.h"
#include "RigidBodyGenerator.h"


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
{
}

void RigidBodySystem::setRenderable(bool renderable)
{
	_isRenderable = renderable;
}

void RigidBodySystem::setDead()
{
	_isDead = true;
    setActive(false);

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

bool RigidBodySystem::mustKillRigidBody(const RigidBody &p, const RigidBodyGenerator &generator) const
{
    return generator.getLifetimePolicy().shouldDelete(generator.getDistribution(), p);
}

bool RigidBodySystem::mustSpawnRigidBody(double deltaTime, const RigidBodyGenerator &generator) const
{
    return generator.getGenerationPolicy().shouldSpawn(generator.getDistribution(), deltaTime);
}

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

void RigidBodySystem::setRenderableForRigidBody(RigidBody &rigidBody)
{
    // std::cout << "isRenderable: " << _isRenderable << " isVisible: " << rigidBody.isVisible() << std::endl;
    if (_isRenderable && !rigidBody.isVisible()) {
        rigidBody.setVisibility(true);
    }
    else if (!_isRenderable) {
        rigidBody.setVisibility(false);
    }
}

void RigidBodySystem::setActivateForceGenAtForceManager(fGenId forceGenId, bool active)
{
    bool result = _forceManager.setActiveForceGenAtForceManager(forceGenId, active);
    if (!result) {
        std::cerr << "ForceGenerator with ID " << forceGenId << " not found in ForceManager." << std::endl;
    }
}

void RigidBodySystem::registerForceGenAtForceManager(std::unique_ptr<ForceGenerator> forceGen)
{
    _registeredForceGenIds.push_back(forceGen->getId());
    _forceManager.registerForceGenerator(_id, std::move(forceGen));
}

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
