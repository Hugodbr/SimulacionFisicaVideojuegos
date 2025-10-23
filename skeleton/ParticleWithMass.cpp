#include "ParticleWithMass.h"


ParticleWithMass::ParticleWithMass(
	const physx::PxTransform& initTransform,
	const physx::PxVec3&      realVelocity,
	const physx::PxVec3&      initAcceleration,
	const physx::PxVec3&      realGravity,
	double realMass,
	double damping,
	Constants::Integration_Method integrationMethod,
	float size,
	double gravityFactor,
	double velocityFactor
)
	: Particle(initTransform, realVelocity, initAcceleration, damping, integrationMethod, size)
{
	_gravityReal  = realGravity;
	_massReal     = realMass;
	_velocityReal = realVelocity;

	_gravityFactor = gravityFactor;
	_velocityFactor = velocityFactor;

	setSimulatedVelocity();
	setSimulatedGravity();
	setSimulatedAcceleration();
	setSimulatedMass();
}

ParticleWithMass::ParticleWithMass(const physx::PxTransform& initTransform, const physx::PxVec3& realVelocity, const physx::PxVec3& initAcceleration, double realMass, float size, double gravityFactor, double velocityFactor, Constants::Integration_Method integrationMethod)
	: ParticleWithMass(initTransform, realVelocity, initAcceleration, Constants::Physics::Gravity, realMass, Constants::Physics::Damping, integrationMethod, size, gravityFactor, velocityFactor)
{
}

ParticleWithMass::ParticleWithMass(const physx::PxTransform& initTransform, const physx::PxVec3& realVelocity, const physx::PxVec3& initAcceleration, double realMass, Constants::Integration_Method integrationMethod)
	: ParticleWithMass(initTransform, realVelocity, initAcceleration, Constants::Physics::Gravity, realMass, Constants::Physics::Damping, integrationMethod)
{
}

ParticleWithMass::ParticleWithMass(const ParticleWithMass& other)
	: Particle(other)
{
	_gravityFactor = other._gravityFactor;
	_velocityFactor = _velocityFactor;

	_gravity = other._gravity;
	_gravityReal = other._gravityReal;

	_velocityReal = other._velocityReal;

	_mass = other._mass;
	_massReal = other._massReal;
}

std::unique_ptr<Particle> ParticleWithMass::clone() const
{
	return std::make_unique<ParticleWithMass>(*this);
}

void ParticleWithMass::setSimulatedVelocity()
{
	_velocity = _velocityReal * _velocityFactor;
}

void ParticleWithMass::setSimulatedGravity()
{
	_gravity = _gravityReal * _gravityFactor;
}

void ParticleWithMass::setSimulatedAcceleration()
{
	_acceleration = _acceleration + _gravity;
}

// Se calcula con base en la Energía Cinética que debe ser igual la real y la simulada
void ParticleWithMass::setSimulatedMass()
{
	_mass = _massReal * pow((_velocityReal.magnitude() / _velocity.magnitude()), 2);
}