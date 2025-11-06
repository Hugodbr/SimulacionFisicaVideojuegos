#include "ParticleWithMass.h"

#include "ForceGenerator.h"

ParticleWithMass::ParticleWithMass(
	const physx::PxTransform& initTransform,
	const physx::PxVec3&      velocity,
	const physx::PxVec3&      initAcceleration,
	float mass,
	float size,
	Constants::Integration_Method integrationMethod,
	float damping
)
	: Particle(initTransform, velocity, initAcceleration, damping, integrationMethod, size)
	, _mass(mass)
{
	init();
}

ParticleWithMass::ParticleWithMass(
	const physx::PxTransform& initTransform,
	const physx::PxVec3&      velocity,
	const physx::PxVec3&      initAcceleration,
	float mass,
	float size,
	Constants::Integration_Method integrationMethod
)
	: Particle(initTransform, velocity, initAcceleration, Constants::Physics::Damping, integrationMethod, size)
	, _mass(mass)
{
	init();
}

ParticleWithMass::ParticleWithMass(float mass, float size, const physx::PxVec4 &color)
	: Particle(size, color, 0.0f, Constants::Integration_Method::VERLET)
	, _mass(mass)
{
	init();
}

ParticleWithMass::ParticleWithMass(float mass, float size, const physx::PxVec4 &color, float speed)
	: Particle(size, color, speed)
	, _mass(mass)
{
	init();
}

ParticleWithMass::ParticleWithMass(const ParticleWithMass& other)
	: Particle(other)
{
	_velocity = other._velocity;

	_mass = other._mass;

	_resultingForce = other._resultingForce;
	_inverseMass = other._inverseMass;
}

void ParticleWithMass::init()
{
	Particle::init();
	_resultingForce = physx::PxVec3(0.0f, 0.0f, 0.0f);
	setInverseMass();
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

void ParticleWithMass::changeMass(float newMass)
{
	_mass = newMass;
	setInverseMass();
}

void ParticleWithMass::setInverseMass()
{
	_inverseMass = (_mass != 0.0) ? 1.0 / _mass : 0.0;
}
