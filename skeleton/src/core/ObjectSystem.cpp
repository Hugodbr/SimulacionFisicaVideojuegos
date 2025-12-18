#include "ObjectSystem.h"

#include "PhysicalObject.h"
#include "ObjectGenerator.h"
#include "ForceGenerator.h"
#include "ForceManager.h"
#include "EntityRenderable.h"


// Initialize static member
uint64_t ObjectSystem::_nextId = 0;

ObjectSystem::ObjectSystem()
    : _id(_nextId++)
    , _emitterOrigin(physx::PxVec3(0, 0, 0))
    , _forceManager(ForceManager::getInstance())
    , _registeredForceGenIds()
    , _isActive(true)
    , _isDead(false)
    , _isRenderable(true)
    , _worldRegion(Region(physx::PxBounds3(
        physx::PxVec3(-1000.0f, -1000.0f, -1000.0f),
        physx::PxVec3(1000.0f, 1000.0f, 1000.0f)
    )))
    , _subSystemActiveCount(0)
    , _maxSubSystems(Constants::System::MaxSubSystems)
{
    _subSystems.reserve(_maxSubSystems);
}

void ObjectSystem::setRenderable(bool renderable)
{
	_isRenderable = renderable;
}

void ObjectSystem::setRenderableForObject(PhysicalObject &object)
{
    if (_isRenderable && !object.isVisible()) {
        object.setVisibility(true);
    }
    else if (!_isRenderable) {
        object.setVisibility(false);
    }
}

void ObjectSystem::setDead()
{
	_isDead = true;
    setActive(false);
    
    for (auto& subSystem : _subSystems) {
        if (subSystem) {
            subSystem->setDead();
        }
    }

    deregisterAllForceGensAtForceManager();
}

bool ObjectSystem::doForceAffectsSystem(const ForceGenerator &forceGen) const
{
    for (const auto &group : _groups) {
        // if (group == Constants::Group::DynamicGroup::NONE) {
        //     return false;
        // }
        // if (forceGen.getGroup() == group 
        //     || forceGen.getGroup() == Constants::Group::DynamicGroup::ALL 
        //     || (forceGen.getObjectSystem() != nullptr && forceGen.getObjectSystem()->getId() == this->getId())) 
        //     {
        //     return true;
        // }
    }
    return false;
}

bool ObjectSystem::mustKillObject(const PhysicalObject &object, const ObjectGenerator &generator) const
{
    return generator.getLifetimePolicy().shouldDelete(generator.getDistribution(), object);
}

bool ObjectSystem::mustSpawnObject(double deltaTime, const ObjectGenerator &generator) const
{
    return generator.getGenerationPolicy().shouldSpawn(generator.getDistribution(), deltaTime);
}

void ObjectSystem::load()
{
    _renderableEntity->load();
}

void ObjectSystem::unload()
{
    _renderableEntity->unload();
}

void ObjectSystem::update(double deltaTime)
{
    if (_isActive) {
        updateSubSystems(deltaTime); 
    }
    else {
        // std::cout << "ObjectSystem " << _id << " is inactive. Skipping update." << std::endl;
    }
}

void ObjectSystem::setWorldRegion(const Region &region)
{
	_worldRegion = Region(region);
}

void ObjectSystem::setRenderableEntity(std::shared_ptr<Abs_Entity> renderable)
{
    _renderableEntity = std::move(renderable);
}

void ObjectSystem::render(const glm::mat4 &modelViewMat)
{
    if (!isActive() || !isRenderable() || !_renderableEntity) {
        return;
    }

    for (const auto& subSystem : _subSystems) {
        if (subSystem && subSystem->isActive() && subSystem->isRenderable()) {
            subSystem->render(modelViewMat);
        }
    }

    onRender(modelViewMat);
}

void ObjectSystem::updateSubSystems(double deltaTime)
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

void ObjectSystem::setActivateForceGenAtForceManager(fGenId forceGenId, bool active)
{
    bool result = _forceManager.setActiveForceGenAtForceManager(forceGenId, active);
    if (!result) {
        std::cerr << "ForceGenerator with ID " << forceGenId << " not found in ForceManager." << std::endl;
    }
}

void ObjectSystem::registerSubSystem(std::unique_ptr<ObjectSystem> subSystem)
{
    _subSystems.push_back(std::move(subSystem));
    _subSystemActiveCount++;
}

void ObjectSystem::registerForceGenAtForceManager(std::unique_ptr<ForceGenerator> forceGen)
{
    _registeredForceGenIds.push_back(forceGen->getId());
    _forceManager.registerForceGenerator(_id, std::move(forceGen));
}

void ObjectSystem::deregisterSubSystem(int subSystemIdx)
{
    assert(subSystemIdx >= 0 && subSystemIdx < _subSystemActiveCount && "Sub-system index out of range");

    std::swap(_subSystems[subSystemIdx], _subSystems[_subSystemActiveCount - 1]);
    _subSystems[_subSystemActiveCount - 1] = nullptr; // Destroys the unique_ptr
    --_subSystemActiveCount;
}

void ObjectSystem::deregisterForceGenAtForceManager(fGenId forceGenId)
{
    _forceManager.deregisterForceGenerator(_id, forceGenId);
}

void ObjectSystem::deregisterAllForceGensAtForceManager()
{
    for (const auto& forceGenId : _registeredForceGenIds) {
        _forceManager.deregisterForceGenerator(_id, forceGenId);
    }
    _registeredForceGenIds.clear();
}
