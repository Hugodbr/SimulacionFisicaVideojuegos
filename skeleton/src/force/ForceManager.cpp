// #include "ForceManager.h"

// #include "ForceGenerator.h"

// void ForceManager::addGlobalForce(std::unique_ptr<ForceGenerator> &forceGen)
// {
//     _globalForces.push_back(std::move(forceGen));
// }

// void ForceManager::clearGlobalForces()
// {
//     _globalForces.clear();
//     _globalResultingForce = physx::PxVec3(0.0f, 0.0f, 0.0f);
// }

// void ForceManager::registerForceGenerator(pSysId systemId, std::unique_ptr<ForceGenerator> &forceGen)
// {
//     _particleSysForcesMap[systemId].push_back(std::move(forceGen));
// }

// void ForceManager::deregisterForceGenerator(pSysId systemId, uint64_t forceGenId)
// {
//     auto itSystem = _particleSysForcesMap.find(systemId);
//     if (itSystem != _particleSysForcesMap.end()) {
//         auto& forceGens = itSystem->second;
//         forceGens.erase(
//             std::remove_if(
//                 forceGens.begin(),
//                 forceGens.end(),
//                 [forceGenId](const std::unique_ptr<ForceGenerator>& fg) {
//                     return fg->getId() == forceGenId;
//                 }
//             ),
//             forceGens.end()
//         );
//     }
// }

// void ForceManager::update(double deltaTime)
// {
// }

// void ForceManager::applyAllForces(double deltaTime)
// {
// }

// void ForceManager::applyGlobalForcesToParticleSystem(pSysId systemId, double deltaTime)
// {
//     computeGlobalForces(deltaTime);

    
// }

// void ForceManager::computeGlobalForces(double deltaTime)
// {
//     for (auto& globalForce : _globalForces) {
//         // _globalResultingForce += globalForce;
//     }
// }

// physx::PxVec3 ForceManager::getGlobalResultingForce() const {
//     return _globalResultingForce;
// }
