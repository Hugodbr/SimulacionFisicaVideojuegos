#pragma once

#include <list>

#include "ParticleWithMass.h"

class ForceGenerator;

class Projectile : public ParticleWithMass
{
public:
    Projectile(
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

	virtual ~Projectile() = default;

	virtual void applyForce(ForceGenerator& forceGenerator) override;
	virtual void setVelocity(const physx::PxVec3& realVelocity) override;

	virtual void changeMass(float newMass) override;


protected:
	virtual void setSimulatedVelocity();
	// virtual void setSimulatedGravity();
	// virtual void setSimulatedAcceleration();
	virtual void setSimulatedMass();

protected:
	float _gravityFactor;
	float _velocityFactor;

	physx::PxVec3 _gravity;
	physx::PxVec3 _gravityReal;

	physx::PxVec3 _velocityReal;

	float _massReal;
};
