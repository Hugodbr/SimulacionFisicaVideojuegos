#pragma once

#include "ParticleWithMass.h"


class RainParticle : public ParticleWithMass
{
public:
	RainParticle(
		const physx::PxTransform& initTransform,
		Constants::Integration_Method integrationMethod = Constants::Integration_Method::VERLET
	);

	virtual ~RainParticle() = default;
};
