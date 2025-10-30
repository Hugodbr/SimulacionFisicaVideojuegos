#pragma once

#include <list>
#include <assert.h>

#include "PxPhysicsAPI.h"

#include "Policies.h"


// class Particle;
// using pID = std::uint64_t;
// using ParticleCollectionPtrs = std::vector<std::unique_ptr<Particle>>;

class ParticleGenerator;
// using GeneratorAndParticlePool = std::pair< std::unique_ptr<ParticleGenerator>, ParticlePool<Particle> >;

// class ForceGenerator;
// using fGenId = std::uint64_t;

// Abstract class
// The System applies the generation rules during the update
class ParticleSystem
{
private:
	// static uint64_t _nextId; // Static counter for generating unique IDs

protected:
	// uint64_t _id; // Protected to allow derived classes access

protected:
	// std::vector<GeneratorAndParticlePool> _generatorAndChildParticlesList;

	std::shared_ptr<Particle> _modelParticle;

	physx::PxVec3 _emitterOrigin;

	// std::vector<uint64_t> _attachedForceGeneratorsIds;

// private:
// 	physx::PxVec3 _commonAcceleration; // e.g., gravity applied to all particles in the system

public:
	ParticleSystem();
	virtual ~ParticleSystem() = default; // smart pointers will auto-clean

	virtual void init() = 0;

	// Calls integrate for each particle in list
	// Verify if has to erase a particle (life, area of interest)
	// Create new particles -> calls generator that returns a list to be incorporated by this system
	virtual void update(double deltaTime) = 0;

	uint64_t getId() const;


protected:
	// virtual void initParticlePools() {};
	// Returns the reserve count per generator for this system
	virtual uint64_t getReserveCountPerGenerator() const;
	// // Creates and registers a particle generator to this system. Don't forget to register it!
	// virtual void createParticleGenerators() = 0;
	// // Registers a particle generator to this system
	// virtual void registerParticleGenerator(std::unique_ptr<ParticleGenerator>& generator);
	// // Adds a particle to this system and associates it to the given generator
	// virtual void addParticles(std::vector<Particle*>& particles, std::unique_ptr<ParticleGenerator>& generator);
	// Determines if a particle must be deleted according to the generator lifetime policies
	virtual bool mustDeleteParticle(const Particle& p, const ParticleGenerator& generator) const;
	// // Removes a particle from this system and from the given generator
	// virtual void removeDeadParticles(std::vector<GeneratorAndChildParticles>::iterator itPG);

	// virtual void createForceGenerators();
	// // Returns true if this system has any force generator attached
	// bool areForceGeneratorsAttached() const;
	// // Adds a force generator to this system and registers it at the ForceManager
	// void addForceGeneratorToSystem(std::unique_ptr<ForceGenerator>& forceGen);

	// // Applies all forces from attached force generators to this system
	// void applyForcesFromGenerators(double deltaTime);
	// // Applies all global forces to this system by updating common acceleration adding global forces times inverse mass
	// void applyGlobalForcesToSystem(double deltaTime);
	// // Applies all forces to all particles in this system
	// void applyForcesToAllParticles(double deltaTime);

	// // Sets a common acceleration (e.g., gravity) applied to all particles in this system
	// void setCommonAcceleration(const physx::PxVec3& force);
	// physx::PxVec3 getCommonAcceleration() const;

	// // Removes a force generator from this system and deregisters it from the ForceManager
	// void removeForceGenFromSystem(fGenId forceGenId);
	// // Removes all force generators from this system and deregisters them from the ForceManager
	// void removeAllForceGensFromSystem();

// private:
	// // Register this system's force generator in the ForceManager
	// void registerForceGenAtForceManager(std::unique_ptr<ForceGenerator>& forceGen);
	// // Deregister this system's force generator from the ForceManager
	// void deregisterForceGenAtForceManager(fGenId forceGenId);
};
