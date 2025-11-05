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

	ParticleWithMass(
		float realMass,
		float size,
		const physx::PxVec4& color,
		float speed
	);

	ParticleWithMass(
		float realMass,
		float size,
		const physx::PxVec4& color
	);

	// Copy constructor deep
	ParticleWithMass(const ParticleWithMass& other);

	virtual ~ParticleWithMass() = default;

	// Sets acceleration to zero and resulting force to zero. Must be called at the beginning of each particle update cycle!
	virtual void clearForces();
	// Adds force to resulting force
	virtual void applyForce(ForceGenerator& forceGenerator);
	// Compute acceleration with resulting force and inverse mass, then call base update. Last step of a particle after forces have been applied.
	// Integrates position and velocity based on acceleration and integration method, and updates age.
	virtual void update(double dt) override;

	virtual void setRealVelocity(const physx::PxVec3& realVelocity);

	virtual physx::PxVec3 getResultingForce() const { return _resultingForce; }
	virtual float getInverseMass() const { return _inverseMass; }
	virtual float getMass() const { return _mass; }

	virtual void changeMass(float newMass);


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
};

