#include "ParticleSystem.h"

#include "ParticleGenerator.h"
// #include "ForceGenerator.h"
#include "ParticleWithMass.h"


// // Initialize static member
// uint64_t ParticleSystem::_nextId = 0;

ParticleSystem::ParticleSystem()
    // : _id(_nextId++)
    : _modelParticle(nullptr)
    // , _generatorAndChildParticlesList()
    , _emitterOrigin(physx::PxVec3(0, 0, 0))
{
    // _generatorAndChildParticlesList.reserve(1); // Default reserve for one generator
}

// void ParticleSystem::registerParticleGenerator(std::unique_ptr<ParticleGenerator> &generator)
// {
//     // ParticleCollectionPtrs emptyChildParticles;
//     // emptyChildParticles.reserve(getReserveCountPerGenerator());
//     // _generatorAndChildParticlesList.push_back({std::move(generator), emptyChildParticles});
// }

// void ParticleSystem::addParticles(std::vector<std::unique_ptr<Particle>> &particles, std::unique_ptr<ParticleGenerator> &generator)
// {
//     // Find the generator in the list
//     auto it = std::find_if(
//         _generatorAndChildParticlesList.begin(),
//         _generatorAndChildParticlesList.end(),
//         [&generator](const GeneratorAndChildParticles& pair) {
//             return pair.first.get() == generator.get();
//         }
//     );
//     // Add to the generator's child particles list
//     if (it != _generatorAndChildParticlesList.end()) {
//         for (auto& particle : particles) {
//             it->second.push_back(std::move(particle));
//         }
//     } else {
//         std::cerr << "Error: Generator not found in ParticleSystem when adding particles." << std::endl;
//     }
// }

bool ParticleSystem::mustDeleteParticle(const Particle &p, const ParticleGenerator &generator) const
{
    return generator.getLifetimePolicy().shouldDelete(generator.getDistribution(), p);
}

// void ParticleSystem::removeDeadParticles(std::vector<GeneratorAndChildParticles>::iterator itGP)
// {
//     auto& childParticles = itGP->second;
//     childParticles.erase(
//         std::remove_if(
//             childParticles.begin(),
//             childParticles.end(),
//             [](const std::unique_ptr<Particle>& p) {
//                 return !p->isActive();
//             }
//         ),
//         childParticles.end()
//     );
// }

// void ParticleSystem::createForceGenerators()
// {
//     // Default: no force generators
// }

// bool ParticleSystem::areForceGeneratorsAttached() const {
//     return !_attachedForceGeneratorsIds.empty();
// }

// void ParticleSystem::addForceGeneratorToSystem(std::unique_ptr<ForceGenerator> &forceGen)
// {
//     _attachedForceGeneratorsIds.push_back(forceGen->getId());
//     registerForceGenAtForceManager(forceGen);
// }

// void ParticleSystem::applyForcesFromGenerators(double deltaTime)
// {

// }

// void ParticleSystem::applyGlobalForcesToSystem(double deltaTime)
// {
//     _commonAcceleration += ForceManager::getInstance().getGlobalResultingForce() * static_cast<float>((std::dynamic_cast<ParticleWithMass>(_modelParticle))->getInverseMass());
// }

// void ParticleSystem::applyForcesToAllParticles(double deltaTime)
// {
//     applyGlobalForcesToSystem(deltaTime);
    
//     if (areForceGeneratorsAttached()) {
//         applyForcesFromGenerators(deltaTime);
//     }	
// }

// void ParticleSystem::setCommonAcceleration(const physx::PxVec3 &force)
// {
//     assert(std::dynamic_pointer_cast<ParticleWithMass>(_modelParticle) && "Model particle must be a ParticleWithMass before setting common acceleration.");

//     _commonAcceleration = force * static_cast<float>((std::dynamic_pointer_cast<ParticleWithMass>(_modelParticle))->getInverseMass());
// }

// physx::PxVec3 ParticleSystem::getCommonAcceleration() const
// {
//     return _commonAcceleration;
// }

// void ParticleSystem::removeForceGenFromSystem(fGenId forceGenId)
// {
//     _attachedForceGeneratorsIds.erase(
//         std::remove(
//             _attachedForceGeneratorsIds.begin(),
//             _attachedForceGeneratorsIds.end(),
//             forceGenId),
//             _attachedForceGeneratorsIds.end()
//         );

//     deregisterForceGenAtForceManager(forceGenId);
// }

// void ParticleSystem::removeAllForceGensFromSystem()
// {
//     for (const auto& forceGenId : _attachedForceGeneratorsIds) {
//         deregisterForceGenAtForceManager(forceGenId);
//     }
//     _attachedForceGeneratorsIds.clear();
// }

// void ParticleSystem::registerForceGenAtForceManager(std::unique_ptr<ForceGenerator> &forceGen)
// {
//     ForceManager::getInstance().registerForceGenerator(_id, std::move(forceGen));
// }

// void ParticleSystem::deregisterForceGenAtForceManager(fGenId forceGenId)
// {
//     ForceManager::getInstance().deregisterForceGenerator(_id, forceGenId);
// }

// uint64_t ParticleSystem::getId() const
// {
//     return _id;
// }

uint64_t ParticleSystem::getReserveCountPerGenerator() const
{
    return 0;
}
