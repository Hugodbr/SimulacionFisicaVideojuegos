#include "CannonBall.h"

CannonBall::CannonBall(const physx::PxTransform& pose, const  physx::PxVec3& realVelocity, const physx::PxVec3& acceleration, const physx::PxVec3& realGravity, double realMass, double damping, Constants::Integration_Method integrationMethod)
	: Particle(pose, realVelocity, acceleration, realGravity, realMass, damping, integrationMethod)
{
	setSimulatedVelocity();
	setSimulatedGravity();
	setSimulatedAcceleration();
	setSimulatedMass();
}

CannonBall::~CannonBall()
{
}

// Se calcula con base en la Energía Cinética que debe ser igual la real y la simulada
void CannonBall::setSimulatedMass()
{
	_mass = _massReal * pow((_velReal.magnitude() / _vel.magnitude()), 2);
}

void CannonBall::setSimulatedVelocity()
{
	_vel = _velReal * _vFactor;
}

void CannonBall::setSimulatedGravity()
{
	_gravity = _gravityReal * _gFactor;
}

void CannonBall::setSimulatedAcceleration()
{
	_acc = _acc + _gravityReal;
}
