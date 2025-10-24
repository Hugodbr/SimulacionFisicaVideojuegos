#include "RainParticle.h"

RainParticle::RainParticle(
	const physx::PxTransform& initTransform,
	const physx::PxVec3& initDirection,
	Constants::Integration_Method integrationMethod
	)
		: ParticleWithMass(
			initTransform,
			initDirection* Constants::Particle::WithMass::Rain::Speed,
			physx::PxVec3(0, 0, 0), // initial acceleration = 0
			Constants::Particle::WithMass::Rain::Mass,
			Constants::Particle::WithMass::Rain::Size,
			Constants::Particle::WithMass::Rain::gFactor,
			Constants::Particle::WithMass::Rain::vFactor,
			integrationMethod
		)
{ }

std::unique_ptr<Particle> RainParticle::clone() const
{
	return std::make_unique<RainParticle>(*this);
}
