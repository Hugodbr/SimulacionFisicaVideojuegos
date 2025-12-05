#include "ForceManager.h"

#include "ForceField.h"
#include "GlobalForce.h"


bool ForceManager::setActiveForceGenAtForceManager(fGenId forceGenId, bool active)
{
    auto it = _forceGenerators.find(forceGenId);
    if (it != _forceGenerators.end()) {
        it->second->setActive(active);
        return true;
    }
    return false;
}

ForceGenerator* ForceManager::registerGlobalForce(std::unique_ptr<ForceGenerator> &forceGen)
{
    uint64_t id = forceGen->getId();
    _forceGenerators[id] = std::move(forceGen);
    _isCacheValid = false; // Invalidate cache

    return _forceGenerators[id].get();
}

ForceGenerator* ForceManager::registerForceGenerator(pSysId systemId, std::unique_ptr<ForceGenerator> forceGen)
{
    uint64_t id = forceGen->getId();
    _forceGenerators[id] = std::move(forceGen);
    _isCacheValid = false; // Invalidate cache

    return _forceGenerators[id].get();
}

void ForceManager::deregisterForceGenerator(pSysId systemId, uint64_t forceGenId)
{
    auto it = _forceGenerators.find(forceGenId);
    if (it != _forceGenerators.end()) {
        _forceGenerators.erase(it);
        _isCacheValid = false; // Invalidate cache
    }
}

const std::vector<ForceGenerator*> &ForceManager::getForceGenerators()
{
    return _cachedForceGenerators;
}

void ForceManager::setWorldLimits(const physx::PxBounds3 &worldLimits)
{
    _worldLimits = worldLimits;
}

void ForceManager::update(double deltaTime)
{
    // Update all force generators, rebuilding cache if invalidated
    if (!_isCacheValid) {
        _cachedForceGenerators.clear();
        // Iterate through the map and update forces, removing the dead force generators, building the cache
        for (auto it = _forceGenerators.begin(); it != _forceGenerators.end(); ) {
            auto& gen = it->second;
            gen->updateForce(deltaTime);
            if (gen->isDead()) {
                it = _forceGenerators.erase(it);
            } else {
                _cachedForceGenerators.push_back(gen.get());
                ++it;
            }
        }
        _isCacheValid = true;
    } 
    // Cache is valid, just update forces
    else {
        for (auto* gen : _cachedForceGenerators) {
            gen->updateForce(deltaTime);
            if (gen->isDead()) {
                _isCacheValid = false; // Invalidate cache to remove dead generators
            }
        }
    }

}
