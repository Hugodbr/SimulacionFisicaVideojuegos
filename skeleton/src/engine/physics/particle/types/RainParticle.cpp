#include "RainParticle.h"


RainParticle::RainParticle(
	const physx::PxTransform& initTransform,
	Constants::Integration_Method integrationMethod
)
	: ParticleWithMass(
        initTransform,
        physx::PxVec3(0, 0, 0), // initial velocity = 0
        Constants::Particle::WithMass::Rain::Mass,
        Constants::Particle::WithMass::Rain::Size,
        integrationMethod,
		Constants::Physics::Damping
	)
{ }
