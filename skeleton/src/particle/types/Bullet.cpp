#include "Bullet.h"

Bullet::Bullet(
	const physx::PxTransform& initTransform, 
	const physx::PxVec3& initDirection, 
	Constants::Integration_Method integrationMethod
)
	: ParticleWithMass(
		initTransform, 
		initDirection * Constants::Particle::WithMass::Bullet::Speed, 
		physx::PxVec3(0, 0, 0),
		Constants::Particle::WithMass::Bullet::Mass, 
		Constants::Particle::WithMass::Bullet::Size, 
		Constants::Particle::WithMass::Bullet::gFactor,
		Constants::Particle::WithMass::Bullet::vFactor,
		integrationMethod
	)
{
}

Bullet::~Bullet()
{
}
