#pragma once
#include "ParticleWithMass.h"


class CannonBall : public ParticleWithMass
{
public:
	CannonBall(
		const physx::PxTransform& initTransform,
		const physx::PxVec3& initDirection,
		Constants::Integration_Method integrationMethod = Constants::Integration_Method::VERLET
	);

	~CannonBall();
};

