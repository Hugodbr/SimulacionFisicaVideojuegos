#include "ForceManager.h"

#include "ForceField.h"
#include "Particle.h"

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

// physx::PxVec3 ForceManager::getGlobalResultingForce() const {
//     return _globalResultingForce;
// }

const physx::PxVec3 &ForceManager::getGlobalResultingForce() const
{
    return _globalResultingForce;
}

void ForceManager::registerGlobalForce(std::unique_ptr<ForceField> forceGen)
{
    _globalForces.push_back(std::move(forceGen));
}

void ForceManager::registerGlobalForceOnParticle(std::unique_ptr<ForceField> forceGen)
{
    _globalForcesOnParticles.push_back(std::move(forceGen));
}

physx::PxVec3 ForceManager::applyGlobalForceOnParticle(Particle &particle, double deltaTime)
{
    physx::PxVec3 totalForce = physx::PxVec3(0.0f, 0.0f, 0.0f);

    for (auto& globalForceOnParticle : _globalForcesOnParticles) {
        totalForce += globalForceOnParticle->computeForceOnParticle(particle);
    }

    return totalForce;
}

void ForceManager::update(double deltaTime)
{
    updateGlobalForces(deltaTime);
    computeGlobalForces(deltaTime);
}

void ForceManager::resetGlobalForces()
{
    _globalForces.clear();
    _globalResultingForce = physx::PxVec3(0.0f, 0.0f, 0.0f);
}

void ForceManager::computeGlobalForces(double deltaTime)
{
    _globalResultingForce = physx::PxVec3(0.0f, 0.0f, 0.0f);

    for (const auto& globalForce : _globalForces) {
        _globalResultingForce += globalForce->getForce();
    }
}

void ForceManager::updateGlobalForces(double deltaTime)
{
    for (auto& globalForce : _globalForces) {
        globalForce->updateForce(deltaTime);
    }
}
