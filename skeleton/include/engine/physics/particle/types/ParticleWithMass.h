#pragma once

#include <list>

#include "Particle.h"

class ForceGenerator;

class ParticleWithMass : public Particle
{
public:

	ParticleWithMass(
		const physx::PxTransform& initTransform, 
		const physx::PxVec3&      velocity,
		float mass,
		float size = Constants::Particle::Default::Size,
		Constants::Integration_Method integrationMethod = Constants::Integration_Method::VERLET,
		float damping = Constants::Physics::Damping
	);

	ParticleWithMass(
		const physx::PxTransform& initTransform, 
		const physx::PxVec3&      velocity,
		float mass,
		float size,
		Constants::Integration_Method integrationMethod = Constants::Integration_Method::VERLET
	);

	ParticleWithMass(
		float mass,
		float size,
		const physx::PxVec4& color
	);

	ParticleWithMass(
		float mass,
		float size,
		const physx::PxVec4& color,
		float speed
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

	virtual physx::PxVec3 getResultingForce() const { return _resultingForce; }
	virtual float getInverseMass() const { return _inverseMass; }
	virtual float getMass() const { return _mass; }

	virtual void changeMass(float newMass);


protected:
	virtual void onInit();
	void setInverseMass();

protected:

	float _mass;
	float _inverseMass;

	physx::PxVec3 _resultingForce = physx::PxVec3(0.0f, 0.0f, 0.0f);
};

