#pragma once

#include <list>

#include "Particle.h"

class ForceGenerator;

class ParticleWithMass : public Particle
{
public:

	ParticleWithMass(
		const physx::PxTransform& initTransform, 
		const physx::PxVec3& realVelocity,
		const physx::PxVec3& initAcceleration,
		const physx::PxVec3& realGravity,
		float realMass,
		float damping,
		Constants::Integration_Method integrationMethod,
		float size = Constants::Particle::Size,
		float gravityFactor = Constants::Particle::Default::gFactor,
		float velocityFactor = Constants::Particle::Default::vFactor
	);

	ParticleWithMass(
		const physx::PxTransform& initTransform, 
		const physx::PxVec3& realVelocity,
		const physx::PxVec3& initAcceleration,
		float realMass,
		float size,
		float gravityFactor,
		float velocityFactor,
		Constants::Integration_Method integrationMethod
	);

	ParticleWithMass(
		const physx::PxTransform& initTransform, 
		const physx::PxVec3& realVelocity,
		const physx::PxVec3& initAcceleration,
		float realMass,
		Constants::Integration_Method integrationMethod
	);

	// Copy constructor
	ParticleWithMass(const ParticleWithMass& other);

	virtual ~ParticleWithMass() = default;

	virtual void clearForces();
	virtual void applyGlobalForces(const physx::PxVec3& globalResultingForce);
	virtual void applyRegisteredForces();

	virtual physx::PxVec3 getResultingForce() const { return _resultingForce; }

	virtual void registerToForce(ForceGenerator& fg);
	virtual void unregisterFromForce(ForceGenerator& fg);
	virtual void unregisterFromAllForces();

	virtual float getInverseMass() const { return _inverseMass; }
	virtual void changeMass(float newMass);

	virtual void update(double dt) override;

protected:

	virtual void setSimulatedVelocity();
	// virtual void setSimulatedGravity();
	// virtual void setSimulatedAcceleration();
	virtual void setSimulatedMass();

	float _gravityFactor;
	float _velocityFactor;

	physx::PxVec3 _gravity;
	physx::PxVec3 _gravityReal;

	physx::PxVec3 _velocityReal;

	float _mass;
	float _massReal;
	float _inverseMass;

	physx::PxVec3 _resultingForce = physx::PxVec3(0.0f, 0.0f, 0.0f);

	std::vector<ForceGenerator*> _registeredForces;
};

