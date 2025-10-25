#pragma once

#include <list>

#include "PxPhysicsAPI.h"

#include "Policies.h"


class Particle;
using pID = std::uint64_t;
using ParticleCollectionPtrs = std::vector<std::unique_ptr<Particle>>;

class ParticleGenerator;
using GeneratorAndChildParticles = std::pair< std::unique_ptr<ParticleGenerator>, ParticleCollectionPtrs>;

class ForceGenerator;
using fGenId = std::uint64_t;

// Abstract class
// The System applies the generation rules during the update
class ParticleSystem
{
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
	// Returns the reserve count per generator for this system
	virtual uint64_t getReserveCountPerGenerator() const = 0;
	// Creates and registers a particle generator to this system. Don't forget to register it!
	virtual void createParticleGenerators() = 0;
	// Registers a particle generator to this system
	virtual void registerParticleGenerator(std::unique_ptr<ParticleGenerator>& generator);
	// Adds a particle to this system and associates it to the given generator
	virtual void addParticles(std::vector<std::unique_ptr<Particle>>& particles, std::unique_ptr<ParticleGenerator>& generator);
	// Determines if a particle must be deleted according to the generator lifetime policies
	virtual bool mustDeleteParticle(const Particle& p, const ParticleGenerator& generator) const;
	// Removes a particle from this system and from the given generator
	virtual void removeDeadParticles(std::vector<GeneratorAndChildParticles>::iterator itPG);

	virtual void createForceGenerators();

	// Adds a force generator to this system and registers it at the ForceManager
	void addForceGeneratorToSystem(std::unique_ptr<ForceGenerator>& forceGen);

	// Removes a force generator from this system and deregisters it from the ForceManager
	void removeForceGenFromSystem(fGenId forceGenId);
	// Removes all force generators from this system and deregisters them from the ForceManager
	void removeAllForceGensFromSystem();

private:
	// Register this system's force generator in the ForceManager
	void registerForceGenAtForceManager(std::unique_ptr<ForceGenerator>& forceGen);
	// Deregister this system's force generator from the ForceManager
	void deregisterForceGenAtForceManager(fGenId forceGenId);

protected:
	std::vector<GeneratorAndChildParticles> _generatorAndChildParticlesList;

	std::shared_ptr<Particle> _modelParticle;

	physx::PxVec3 _emitterOrigin;

	std::vector<uint64_t> _attachedForceGeneratorsIds;

protected:
	uint64_t _id; // Protected to allow derived classes access

private:
    static uint64_t _nextId; // Static counter for generating unique IDs

};
