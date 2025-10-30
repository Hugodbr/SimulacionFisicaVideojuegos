#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>


#include <PxPhysicsAPI.h>

#include "ForceField.h"

using pSysId = std::uint64_t;
// using ForceCollectionPointers = std::vector<std::unique_ptr<ForceGenerator>>;

class ForceManager
{
public:

    inline static ForceManager& getInstance() {
        static ForceManager instance;
        return instance;
    }

    const physx::PxVec3& getGlobalResultingForce() const;

    // void addGlobalForce(std::unique_ptr<ForceGenerator>& forceGen);
    // void clearGlobalForces();

    // // Adds a force generator to the specified particle system indexed by systemId
    // void registerForceGenerator(pSysId systemId, std::unique_ptr<ForceGenerator>& forceGen);
    // // Removes a force generator from the specified particle system indexed by systemId
    // void deregisterForceGenerator(pSysId systemId, uint64_t forceGenId);

    void registerGlobalForce(std::unique_ptr<ForceField> forceGen);

    // // Caution: returns a pointer to internal data structure. Objects can be modified but not deleted(vector is const).
    // // If no force generators for that system, returns nullptr.
    // const ForceCollectionPointers* getForceGenerators(pSysId systemId) const;

    void update(double deltaTime);

    // // Applies all forces registered for a particle system to its particles to all particle systems
    // void applyAllForces(double deltaTime);
    // // Applies all 

    // void applyGlobalForcesToParticleSystem(pSysId systemId, double deltaTime);
    // physx::PxVec3 getGlobalResultingForce() const;

private:
    ForceManager() = default;
    ~ForceManager() = default;

    ForceManager(const ForceManager&) = delete;
    ForceManager& operator=(const ForceManager&) = delete;

    void resetGlobalForces();
    void computeGlobalForces(double deltaTime);
    void updateGlobalForces(double deltaTime);

private:
    std::vector<std::unique_ptr<ForceField>> _globalForces = {}; // List of global forces applied to all particle systems
    physx::PxVec3 _globalResultingForce = physx::PxVec3(0.0f, 0.0f, 0.0f);

    // std::unordered_map<pSysId, ForceCollectionPointers> _particleSysForcesMap; // Map of particle system IDs to their associated force generators
};