#pragma once
#include "ParticleWithMass.h"


class Bullet : public ParticleWithMass
{
public:
	Bullet(
		const physx::PxTransform& initTransform, 
		const physx::PxVec3& initDirection, 
		Constants::Integration_Method integrationMethod = Constants::Integration_Method::VERLET
	);

	~Bullet();
};
