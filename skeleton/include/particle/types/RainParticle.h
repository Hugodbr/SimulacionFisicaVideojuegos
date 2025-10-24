#pragma once
#include "ParticleWithMass.h"


class RainParticle : public ParticleWithMass
{
public:
	RainParticle(
		const physx::PxTransform& initTransform,
		const physx::PxVec3& initDirection,
		Constants::Integration_Method integrationMethod = Constants::Integration_Method::VERLET
	);

	~RainParticle() = default;

	// Returns a new Particle that is a clone from this one
	std::unique_ptr<Particle> clone() const override;
};
