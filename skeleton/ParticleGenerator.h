#pragma once

#include <list>
#include <random>

#include <PxPhysicsAPI.h>

#include "Policies.h"

#include "RainParticle.h"

// Abstract class
// The Generator handles spawn and death rules of particles based on probabilistics
// Derived classes implement their own probability distribution
class ParticleGenerator
{
public:

	ParticleGenerator();

	virtual ~ParticleGenerator() = default;

	virtual void init(
		const physx::PxVec3& emitterOrigin,
		const Vector3Stats& velocity,
		Particle* modelParticle
		//std::shared_ptr<Particle> modelParticle
	);

	//virtual std::list<std::unique_ptr<Particle>> generateParticles(double deltaTime);
	//virtual std::unique_ptr<RainParticle> generateParticle();
	//virtual std::list<Particle*> generateParticles(double deltaTime);
	virtual std::vector<Particle*> generateParticles(double deltaTime);


	// Setters
	virtual void setEmitterOrigin(const physx::PxVec3& emitterOrigin);

	virtual void setMeanVelocity(const physx::PxVec3& meanVel);
	virtual void setVelocityDeviation(const physx::PxVec3& velDeviation);

	//virtual void setModelParticle(std::shared_ptr<Particle> model);
	virtual void setModelParticle(Particle* model);

	// Getters
	virtual double getDistribution() const = 0;

	virtual physx::PxVec3 getEmitterOrigin() const;

	virtual physx::PxVec3 getMeanVelocity() const;
	virtual physx::PxVec3 getVelocityDeviation() const;

	virtual const Particle& getModelParticle() const;


protected:

	mutable std::mt19937 _mt;

	physx::PxVec3 _emitterOrigin;

	physx::PxVec3 _meanVelocity;
	physx::PxVec3 _velocityDeviation;

	double accumulator = 0.0;

	//std::shared_ptr<Particle> _modelParticle; // Can be change by another in run time with this implementation
	Particle* _modelParticle; // Can be change by another in run time with this implementation

};
