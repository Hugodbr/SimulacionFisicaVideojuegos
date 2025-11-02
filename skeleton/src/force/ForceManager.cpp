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

void ForceManager::registerGlobalForce(std::unique_ptr<GlobalForce> &forceGen)
{
    _forceGenerators[forceGen->getId()] = std::move(forceGen);
    _isCacheValid = false; // Invalidate cache
}

void ForceManager::registerForceGenerator(pSysId systemId, std::unique_ptr<ForceGenerator> &forceGen)
{
    _forceGenerators[forceGen->getId()] = std::move(forceGen);
    _isCacheValid = false; // Invalidate cache
}

void ForceManager::deregisterForceGenerator(pSysId systemId, uint64_t forceGenId)
{
    auto it =  _forceGenerators.find(forceGenId);
    if (it != _forceGenerators.end()) {
        _forceGenerators.erase(it);
        _isCacheValid = false; // Invalidate cache
    }
}

const std::vector<ForceGenerator *> &ForceManager::getForceGenerators()
{
    return _cachedForceGenerators;
}

void ForceManager::update(double deltaTime)
{
    // Update all force generators, rebuilding cache if invalidated
    if (!_isCacheValid) {
        _cachedForceGenerators.clear();
        for (auto& [id, gen] : _forceGenerators) {
            gen->updateForce(deltaTime);
            _cachedForceGenerators.push_back(gen.get());
        }
        _isCacheValid = true;
    } 
    // Cache is valid, just update forces
    else {
        for (auto* gen : _cachedForceGenerators) {
            gen->updateForce(deltaTime);
        }
    }

}
