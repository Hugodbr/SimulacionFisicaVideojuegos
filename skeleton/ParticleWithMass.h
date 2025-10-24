#pragma once
#include "Particle.h"


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

	// Copy constructor deletion
	ParticleWithMass(const ParticleWithMass& other) = delete;
	ParticleWithMass& operator=(const ParticleWithMass&) = delete;

	virtual ~ParticleWithMass() = default;

	// Returns a new Particle that is a clone from this one
	//virtual std::unique_ptr<Particle> clone() const override;
	virtual ParticleWithMass* clone() const;

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
};

