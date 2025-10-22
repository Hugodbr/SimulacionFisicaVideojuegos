#pragma once
#include <list>

#include "PxPhysicsAPI.h"

#include "Policies.h"


class Particle;
class ParticleGenerator;

using GeneratorAndChildParticles = std::pair< std::unique_ptr<ParticleGenerator>, std::list<std::unique_ptr<Particle>> >;

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


protected:

	virtual bool mustDelete(const Particle& p, const ParticleGenerator& gen) const;


protected:

	std::list<GeneratorAndChildParticles> _generatorAndChildParticlesList;

	std::unique_ptr<Particle> _modelParticle;

	physx::PxVec3 _emitterOrigin;
};
