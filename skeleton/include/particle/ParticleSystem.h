#pragma once

#include <list>
#include <assert.h>

#include "PxPhysicsAPI.h"

#include "Policies.h"
#include "ParticlePool.h"

class ForceManager;
class ForceGenerator;
class ParticleGenerator;
class Particle;
class ParticleWithMass;

// class ForceGenerator;
using fGenId = std::uint64_t;

// Abstract class
// The System applies the generation rules during the update
class ParticleSystem
{
private:
	static uint64_t _nextId; // Static counter for generating unique IDs

protected:
	uint64_t _id; // Protected to allow derived classes access

protected:
	physx::PxVec3 _emitterOrigin;

	ForceManager& _forceManager;

	std::vector<fGenId> _registeredForceGenIds; // IDs of force generators registered at ForceManager that originate from this system

	std::vector<std::unique_ptr<ForceGenerator>> _forceGensInsideSystem;
	int _forceGenActiveCount;
	uint16_t _maxInsideForceGenerators;

	std::vector<std::unique_ptr<ParticleSystem>> _subSystems;
	int _subSystemActiveCount;
	uint16_t _maxSubSystems;

	bool _isRenderable;
	bool _isActive;
	bool _isDead;

public:
	ParticleSystem();
	virtual ~ParticleSystem() = default; // smart pointers will auto-clean

	virtual void init() = 0;

	virtual void update(double deltaTime);

	uint64_t getId() const { return _id; }

	// Check if the system is renderable
	bool isRenderable() const { return _isRenderable; }
	// Set if the system is renderable
	virtual void setRenderable(bool renderable);
	// If the system is renderable, set renderable state a particle
	// Used when activating particles at the system.
	virtual void setRenderableForParticle(Particle& particle);
	// Active systems are updated. Inactive systems skip update, but remain in the simulation and can be re-activated. The system won't responde to set renderable calls while inactive!
	bool isActive() const { return _isActive; }
	void setActive(bool active) { _isActive = active; }
	// Dead systems are removed from the simulation
	bool isDead() const { return _isDead; }
	void setDead();


protected:
	// Returns the reserve count per generator for this system
	virtual uint64_t getReserveCountPerGenerator() const = 0;

	// Determines if a particle must be deleted according to the generator lifetime policies
	virtual bool mustKillParticle(const Particle& p, const ParticleGenerator& generator) const;

	virtual bool mustSpawnParticle(double deltaTime, const ParticleGenerator& generator) const;

	virtual void applyForceManagerForces(ParticleWithMass& particle, const std::vector<ForceGenerator*>& forceGenerators);
	virtual void applyInsideForces(ParticleWithMass& particle);

	// Updates all sub-systems registered inside this particle system.
	// The method also removes dead sub-systems.
	virtual void updateSubSystems(double deltaTime);
	// Updates all force generators registered inside this particle system. Must be called before applying forces to particles.
	// The method also removes dead force generators.
	virtual void updateInsideForces(double deltaTime);

	// ACTIVATE / DEACTIVATE METHODS for ForceGenerators. Can be any at ForceManager.
	// Deactivate inside force generator
	void setActivateInsideForceGen(int forceGenIdx, bool active);
	// Deactivate a force generator registered at ForceManager
	void setActivateForceGenAtForceManager(fGenId forceGenId, bool active);


	// REGISTER METHODS
	// Register a sub-system inside this particle system.
	// The ownership of the sub-system is retained by the system.
	void registerSubSystem(std::unique_ptr<ParticleSystem>& subSystem);
	// Register a force generator to be used only inside this particle system.
	// The ownership of the force generator is retained by the system.
	void registerInsideForceGen(std::unique_ptr<ForceGenerator>& forceGen);
	// Register this system's force generator in the ForceManager. The force is now seen by all systems.
	// The ownership of the force generator is transferred to the ForceManager.
	// The system only keeps the ID of the registered force generator.
	void registerForceGenAtForceManager(std::unique_ptr<ForceGenerator>& forceGen);

	// DEREGISTER METHODS - deregistration also deletes the object!
	// Deregister a sub-system inside this particle system. 
	// The sub-system is deleted.
	void deregisterSubSystem(int subSystemIdx);
	// Deregister a force generator used only inside this particle system. 
	// The force generator is deleted.
	void deregisterInsideForceGen(int forceGenIdx);
	// Deregister all force generators used only inside this particle system. 
	// Force generators will be deleted.
	void deregisterAllInsideForceGens();
	// Deregister this system's force generator from the ForceManager.
	void deregisterForceGenAtForceManager(fGenId forceGenId);
	// Deregister all force generators from the ForceManager that were registered by this system.
	// Force generators will be deleted. This will clear the influence of this system's forces on all particle systems.
	void deregisterAllForceGensAtForceManager();




};
