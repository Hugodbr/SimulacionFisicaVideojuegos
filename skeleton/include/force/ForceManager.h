#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>

#include <PxPhysicsAPI.h>


class ForceGenerator;
class GlobalForce;

using pSysId = std::uint64_t;
using fGenId = std::uint64_t;

class ForceManager
{
public:

    inline static ForceManager& getInstance() {
        static ForceManager instance;
        return instance;
    }


    // ACTIVATE / DEACTIVATE 
    bool setActiveForceGenAtForceManager(fGenId forceGenId, bool active);

    // Register forces that aren't associated with a specific particle system
    void registerGlobalForce(std::unique_ptr<GlobalForce>& forceGen);

    // Adds a force generator to the specified particle system indexed by systemId
    void registerForceGenerator(pSysId systemId, std::unique_ptr<ForceGenerator>& forceGen);
    // Removes a force generator from the specified particle system indexed by systemId
    void deregisterForceGenerator(pSysId systemId, uint64_t forceGenId);

    // Getter for all force generators. The returned vector is valid until the next modification (addition/removal) of force generators.
    // The cached vector is updated only at the manager update, so the construction is made only once per update cycle. Otherwise,
    // systems that update first would have an advantage by making their generators available sooner.
    const std::vector<ForceGenerator*>& getForceGenerators();

    // Update all force generators managed by this manager
    // Builds the cached vector if invalidated
    void update(double deltaTime);


private:
    ForceManager() = default;
    ~ForceManager() = default;

    ForceManager(const ForceManager&) = delete;
    ForceManager& operator=(const ForceManager&) = delete;


private:

    std::unordered_map<fGenId, std::unique_ptr<ForceGenerator>> _forceGenerators;

    // Cached list of force generators for quick iteration during update. The map is not ideal for iteration, so the force manager
    // at its update step builds a vector of pointers to the force generators to avoid repeated map lookups.
    // If a new force generator is added or removed, the cache is invalidated and rebuilt at the next update.
    bool _isCacheValid = false;
    std::vector<ForceGenerator*> _cachedForceGenerators;

};