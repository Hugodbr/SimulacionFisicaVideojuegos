#pragma once
#include "Projectile.h"


class CannonBall : public Projectile
{
public:
	CannonBall(
        const physx::PxTransform& initTransform, 
        const physx::PxVec3&      realVelocity,
        float realMass,
        float size,
        const physx::PxVec4& color,
        float damping = Constants::Physics::Damping,
        float velocityFactor = Constants::Particle::Default::vFactor,
        float gravityFactor = Constants::Particle::Default::gFactor,
        Constants::Integration_Method integrationMethod = Constants::Integration_Method::VERLET
    );

	virtual ~CannonBall() = default;
};

