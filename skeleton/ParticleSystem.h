#pragma once
#include <list>

#include "PxPhysicsAPI.h"


struct ParticleLifetimePolicy;

class Particle;
class ParticleGenerator;

// Abstract class
// The System applies the generation rules during the update
class ParticleSystem
{
public:
	ParticleSystem();
	virtual ~ParticleSystem();

	virtual void init() = 0;

	// Calls integrate for each particle in list
	// Verify if has to erase a particle (life, area of interest)
	// Create new particles -> calls generator that returns a list to be incorporated by this system
	virtual void update(double dt) = 0;

protected:

	virtual bool shouldDelete(const Particle& p, const ParticleLifetimePolicy& policy);


protected:

	std::list<Particle*> _particleList;
	std::list<ParticleGenerator*> _generatorList;

	physx::PxVec3 _origin;
	double _radius;

	physx::PxVec3 _averagePosition;
	physx::PxVec3 _averageVelocity;
	double _averageDuration;
	double _generationProbability;
	Particle* _modelParticle;
};

