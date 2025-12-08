#pragma once
#include "Projectile.h"


class Bullet : public Projectile
{
public:
	Bullet(
        const physx::PxTransform& initTransform, 
        const physx::PxVec3&      realVelocity,
        float realMass = Constants::Particle::WithMass::Bullet::Mass,
        float size = Constants::Particle::WithMass::Bullet::Size,
        const physx::PxVec4& color = Constants::Particle::WithMass::Bullet::Color,
        float damping = Constants::Physics::Damping,
        float velocityFactor = Constants::Particle::Default::vFactor,
        float gravityFactor = Constants::Particle::Default::gFactor,
        Constants::Integration_Method integrationMethod = Constants::Integration_Method::VERLET
    );

	~Bullet() override = default;
};
