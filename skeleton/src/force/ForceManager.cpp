#include "ForceManager.h"

#include "ForceGenerator.h"

void ForceManager::registerForceGenerator(pSysId systemId, std::unique_ptr<ForceGenerator> forceGen)
{
    _particleSysForcesMap[systemId].push_back(std::move(forceGen));
}

void ForceManager::deregisterForceGenerator(pSysId systemId, uint64_t forceGenId)
{
    auto it = _particleSysForcesMap.find(systemId);
    if (it != _particleSysForcesMap.end()) {
        auto& forceGens = it->second;
        forceGens.erase(
            std::remove_if(
                forceGens.begin(),
                forceGens.end(),
                [forceGenId](const std::unique_ptr<ForceGenerator>& fg) {
                    return fg->getId() == forceGenId;
                }
            ),
            forceGens.end()
        );
    }
}

const ForceCollectionPointers* ForceManager::getForceGenerators(pSysId systemId) const
{
    auto it = _particleSysForcesMap.find(systemId);
    if (it != _particleSysForcesMap.end()) {
        return &(it->second);
    }
    
    return nullptr;
}

void ForceManager::applyAllForces(double deltaTime)
{
}

void ForceManager::applyForcesToParticleSystem(pSysId systemId, double deltaTime)
{
    
}
