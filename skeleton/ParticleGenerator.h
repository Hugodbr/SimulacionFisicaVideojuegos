#pragma once

#include <list>
#include <random>

#include <PxPhysicsAPI.h>

#include "Policies.h"


class Particle;

// Abstract class
// The Generator handles spawn and death rules of particles
class ParticleGenerator
{
public:

	ParticleGenerator();
	virtual ~ParticleGenerator();

	virtual void init(
		const physx::PxVec3& emitterOrigin,
		const Vector3Stats& velocity,
		const Particle* modelParticle
	);

	virtual std::list<Particle*> generateParticles(double deltaTime) = 0;

	// Setters
	virtual void setEmitterOrigin(const physx::PxVec3& emitterOrigin);

	virtual void setMeanVelocity(const physx::PxVec3& meanVel);
	virtual void setVelocityDeviation(const physx::PxVec3& velDeviation);

	virtual void setModelParticle(const Particle* model);

	virtual void setGenerationPolicy(const ParticleGenerationPolicy& genPolicy);
	virtual void setLifetimePolicy(const ParticleLifetimePolicy& lifePolicy);

	// Getters
	virtual physx::PxVec3 getEmitterOrigin() const;

	virtual physx::PxVec3 getMeanVelocity() const;
	virtual physx::PxVec3 getVelocityDeviation() const;

	virtual const Particle& getModelParticle() const;

	virtual ParticleGenerationPolicy& getGenerationPolicy() const;
	virtual ParticleLifetimePolicy& getLifetimePolicy() const;


protected:

	std::mt19937 _mt;

	physx::PxVec3 _emitterOrigin;

	physx::PxVec3 _meanVelocity;
	physx::PxVec3 _velocityDeviation;

	const Particle* _modelParticle;

	ParticleGenerationPolicy* _generationPolicy;
	ParticleLifetimePolicy* _lifetimePolicy;
};
