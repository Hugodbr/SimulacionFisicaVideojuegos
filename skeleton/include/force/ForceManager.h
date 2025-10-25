#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>


#include <PxPhysicsAPI.h>


class ForceGenerator;

using pSysId = std::uint64_t;
using ForceCollectionPointers = std::vector<std::unique_ptr<ForceGenerator>>;

class ForceManager
{
public:

    inline static ForceManager& getInstance() {
        static ForceManager instance;
        return instance;
    }

    // Adds a force generator to the specified particle system indexed by systemId
    void registerForceGenerator(pSysId systemId, std::unique_ptr<ForceGenerator> forceGen);

    void deregisterForceGenerator(pSysId systemId, uint64_t forceGenId);

    // Caution: returns a pointer to internal data structure. Objects can be modified but not deleted(vector is const).
    // If no force generators for that system, returns nullptr.
    const ForceCollectionPointers* getForceGenerators(pSysId systemId) const;

    void update(double deltaTime);

private:
    ForceManager() = default;
    ~ForceManager() = default;

    ForceManager(const ForceManager&) = delete;
    ForceManager& operator=(const ForceManager&) = delete;

    void applyAllForces(double deltaTime);
    void applyForcesToParticleSystem(pSysId systemId, double deltaTime);
private:
    // physx::PxVec3 _globalForces = physx::PxVec3(0.0f, 0.0f, 0.0f);

    std::unordered_map<pSysId, ForceCollectionPointers> _particleSysForcesMap; // Map of particle system IDs to their associated force generators

};