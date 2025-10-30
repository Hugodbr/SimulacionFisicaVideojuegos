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
		double realMass,
		double damping,
		Constants::Integration_Method integrationMethod,
		float size = Constants::Particle::Size,
		double gravityFactor = Constants::Particle::Default::gFactor,
		double velocityFactor = Constants::Particle::Default::vFactor
	);

	ParticleWithMass(
		const physx::PxTransform& initTransform, 
		const physx::PxVec3& realVelocity,
		const physx::PxVec3& initAcceleration,
		double realMass,
		float size,
		double gravityFactor,
		double velocityFactor,
		Constants::Integration_Method integrationMethod
	);

	ParticleWithMass(
		const physx::PxTransform& initTransform, 
		const physx::PxVec3& realVelocity,
		const physx::PxVec3& initAcceleration,
		double realMass,
		Constants::Integration_Method integrationMethod
	);

	// Copy constructor
	ParticleWithMass(const ParticleWithMass& other);

	virtual ~ParticleWithMass() = default;

	// Returns a new Particle that is a clone from this one
	virtual std::unique_ptr<Particle> clone() const override;

	// // virtual void addForce(const physx::PxVec3& force); // TODO
	// virtual void clearForces();
	// virtual physx::PxVec3 getResultingForce();

	// virtual void registerToForce(ForceGenerator& fg);
	// virtual void unregisterFromForce(ForceGenerator& fg);
	// virtual void unregisterFromAllForces();

	virtual double getInverseMass() const;
	virtual void changeMass(double newMass);

	virtual void update(double dt) override;

protected:

	virtual void setSimulatedVelocity();
	virtual void setSimulatedGravity();
	virtual void setSimulatedAcceleration();
	virtual void setSimulatedMass();

	double _gravityFactor;
	double _velocityFactor;

	physx::PxVec3 _gravity;
	physx::PxVec3 _gravityReal;

	physx::PxVec3 _velocityReal;

	double _mass;
	double _massReal;
	double _inverseMass;

	// std::vector<ForceGenerator&> _forceGenerators = {};
	// physx::PxVec3 _resultingForce = physx::PxVec3(0.0f, 0.0f, 0.0f);
};

