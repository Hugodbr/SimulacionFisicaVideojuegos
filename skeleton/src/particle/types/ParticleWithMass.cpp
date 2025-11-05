#include "ParticleWithMass.h"

#include "ForceGenerator.h"

ParticleWithMass::ParticleWithMass(
	const physx::PxTransform& initTransform,
	const physx::PxVec3&      realVelocity,
	const physx::PxVec3&      initAcceleration,
	const physx::PxVec3&      realGravity,
	float realMass,
	float damping,
	Constants::Integration_Method integrationMethod,
	float size,
	float gravityFactor,
	float velocityFactor
)
	: Particle(initTransform, realVelocity, initAcceleration, damping, integrationMethod, size)
{
	_gravityReal  = realGravity;
	_massReal     = realMass;
	_velocityReal = realVelocity;

	_gravityFactor = gravityFactor;
	_velocityFactor = velocityFactor;

	_resultingForce = physx::PxVec3(0.0f, 0.0f, 0.0f);

	setSimulatedVelocity();
	// setSimulatedGravity();
	// setSimulatedAcceleration();
	setSimulatedMass();
}

ParticleWithMass::ParticleWithMass(const physx::PxTransform& initTransform, const physx::PxVec3& realVelocity, const physx::PxVec3& initAcceleration, float realMass, float size, float gravityFactor, float velocityFactor, Constants::Integration_Method integrationMethod)
	: ParticleWithMass(initTransform, realVelocity, initAcceleration, Constants::Physics::Gravity, realMass, Constants::Physics::Damping, integrationMethod, size, gravityFactor, velocityFactor)
{
}

ParticleWithMass::ParticleWithMass(const physx::PxTransform& initTransform, const physx::PxVec3& realVelocity, const physx::PxVec3& initAcceleration, float realMass, Constants::Integration_Method integrationMethod)
	: ParticleWithMass(initTransform, realVelocity, initAcceleration, Constants::Physics::Gravity, realMass, Constants::Physics::Damping, integrationMethod)
{
}

ParticleWithMass::ParticleWithMass(float realMass, float size, const physx::PxVec4 &color, float speed)
	: Particle(physx::PxTransform(physx::PxVec3(0, 0, 0), physx::PxQuat(0)), physx::PxVec3(0, 0, 0), physx::PxVec3(0, 0, 0), Constants::Integration_Method::VERLET, size, Constants::Physics::Damping, color)
	, _massReal(realMass)
{
	_velocityReal = physx::PxVec3(1, 0, 0).getNormalized() * speed;

	_resultingForce = physx::PxVec3(0.0f, 0.0f, 0.0f);

	setSimulatedMass();
}

ParticleWithMass::ParticleWithMass(float realMass, float size, const physx::PxVec4 &color)
	: Particle(physx::PxTransform(physx::PxVec3(0, 0, 0), physx::PxQuat(0)), physx::PxVec3(0, 0, 0), physx::PxVec3(0, 0, 0), Constants::Integration_Method::VERLET, size, Constants::Physics::Damping, color)
	, _massReal(realMass)
{
	_resultingForce = physx::PxVec3(0.0f, 0.0f, 0.0f);

	setSimulatedMass();
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

	_resultingForce = other._resultingForce;
	_inverseMass = other._inverseMass;
}

void ParticleWithMass::setRealVelocity(const physx::PxVec3 &realVelocity)
{
	_velocityReal = realVelocity;
	setSimulatedVelocity();
}

void ParticleWithMass::clearForces()
{
	_resultingForce = physx::PxVec3(0.0f, 0.0f, 0.0f);
	_acceleration = physx::PxVec3(0.0f, 0.0f, 0.0f);
}

void ParticleWithMass::applyForce(ForceGenerator& forceGenerator)
{
	// std::cout << "Applying force from ForceGenerator ID " << forceGenerator.getId() << " to Particle ID " << getId() << std::endl;
	// std::cout << "Current resulting force before applying: (" << _resultingForce.x << ", " << _resultingForce.y << ", " << _resultingForce.z << ")" << std::endl;
	// std::cout << "Force to apply: (" << forceGenerator.getForceOnParticle(*this).x << ", " << forceGenerator.getForceOnParticle(*this).y << ", " << forceGenerator.getForceOnParticle(*this).z << ")" <<  std::endl;
	
	_resultingForce += forceGenerator.getForceOnParticle(*this);
}

void ParticleWithMass::update(double dt)
{
	_acceleration += _resultingForce * _inverseMass;
	
	// std::cout << "Particle ID " << getId() << " Acceleration after applying forces: (" << _acceleration.x << ", " << _acceleration.y << ", " << _acceleration.z << ")" << std::endl;

	Particle::update(dt);
}

void ParticleWithMass::setSimulatedVelocity()
{
	_velocity = _velocityReal * _velocityFactor;
	_speed = _velocity.magnitude();
}

// Calculate simulated mass based on kyinetic energy equivalence
void ParticleWithMass::setSimulatedMass()
{
	if (_velocity.magnitude() < Constants::Math::epsilon) {
		_mass = _massReal;
	} else {
		_mass = _massReal * pow((_velocityReal.magnitude() / _velocity.magnitude()), 2);
	}
	_inverseMass = (_mass != 0.0) ? 1.0 / _mass : 0.0;
}

void ParticleWithMass::changeMass(float newMass) {
	_massReal = newMass;
	setSimulatedMass();
}

// void ParticleWithMass::setSimulatedGravity()
// {
// 	_gravity = _gravityReal * _gravityFactor;
// }

// void ParticleWithMass::setSimulatedAcceleration()
// {
// 	_acceleration = _acceleration + _gravity;
// }