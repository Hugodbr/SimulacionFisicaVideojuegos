#pragma once

#include <list>
#include <random>

#include <PxPhysicsAPI.h>

#include "Policies.h"


// Abstract class
// The Generator handles spawn and death rules of objects based on probabilistics
// Derived classes implement their own probability distribution
class ObjectGenerator
{
public:
	ObjectGenerator();
	virtual ~ObjectGenerator() = default;

	virtual void init(
		const physx::PxVec3& emitterOrigin,
		const Vector3Stats& velocity = Vector3Stats()
	);

	virtual int numberOfGenerations();
	virtual physx::PxVec3 getGeneratedPosition();
	virtual physx::PxVec3 getGeneratedVelocity();
	virtual physx::PxVec4 getGeneratedColor();

	// Setters
	virtual void setEmitterOrigin(const physx::PxVec3& emitterOrigin);

	virtual void setMeanVelocity(const physx::PxVec3& meanVel);
	virtual void setVelocityDeviation(const physx::PxVec3& velDeviation);
	
	virtual void setGenerationPolicy(const ObjectGenerationPolicy& genPolicy);
	virtual void setLifetimePolicy(const ObjectLifetimePolicy& lifePolicy);

	// Getters
	virtual double getDistribution() const = 0;

	virtual physx::PxVec3 getEmitterOrigin() const { return _emitterOrigin; }
	virtual physx::PxVec3 getMeanVelocity() const { return _meanVelocity; }
	virtual physx::PxVec3 getVelocityDeviation() const { return _velocityDeviation; }

	virtual ObjectGenerationPolicy& getGenerationPolicy() const { return *_generationPolicy; }
	virtual ObjectLifetimePolicy& getLifetimePolicy() const { return *_lifetimePolicy; }


protected:
	mutable std::mt19937 _mt;

	physx::PxVec3 _emitterOrigin;

	physx::PxVec3 _meanVelocity;
	physx::PxVec3 _velocityDeviation;

	std::unique_ptr<ObjectGenerationPolicy>  _generationPolicy;
	std::unique_ptr<ObjectLifetimePolicy>    _lifetimePolicy;
};
