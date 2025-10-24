#include "CannonBall.h"

CannonBall::CannonBall(
	const physx::PxTransform& initTransform,
	const physx::PxVec3& initDirection,
	Constants::Integration_Method integrationMethod
)
	: ParticleWithMass(
		initTransform,
		initDirection* Constants::Particle::WithMass::CannonBall::Speed,
		physx::PxVec3(0, 0, 0),
		Constants::Particle::WithMass::CannonBall::Mass,
		Constants::Particle::WithMass::CannonBall::Size,
		Constants::Particle::WithMass::CannonBall::gFactor,
		Constants::Particle::WithMass::CannonBall::vFactor,
		integrationMethod)
{
}

CannonBall::~CannonBall()
{
}
