#pragma once

#include <list>
#include <random>

#include <PxPhysicsAPI.h>


class Particle;

// Abstract class
class ParticleGenerator
{
public:
	ParticleGenerator();
	virtual ~ParticleGenerator();

	virtual void init(
		const physx::PxVec3& origin, 
		const physx::PxVec3& velocity, 
		double duration, 
		const Particle& modelParticle
	) = 0;

	virtual std::list<Particle*> generateParticles() = 0;

	// Setters
	virtual void setOrigin(const physx::PxVec3& origin) = 0;
	virtual void setVelocity(const physx::PxVec3& velocity) = 0;
	virtual void setDuration(double duration) = 0;
	virtual void setGenerationProbability(double probability) = 0;
	virtual void setModelParticle(const Particle& modelParticle) = 0;

	// Getters
	virtual physx::PxVec3 getOrigin() const;
	virtual physx::PxVec3 getVelocity() const;
	virtual double getDuration() const;
	virtual double getGenerationProbability() const;
	virtual const Particle& getModelParticle() const;

protected:
	// Random
	std::mt19937 _mt;

	Particle* _modelParticle;

	physx::PxVec3 _origin;
	physx::PxVec3 _velocity;
	double _duration;
	double _probability;
};

