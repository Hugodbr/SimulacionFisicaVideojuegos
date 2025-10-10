#pragma once
#include "ParticleSystem.h"

#include <PxPhysicsAPI.h>


class RainSystem : public ParticleSystem
{
public:
	// Origin = center
	// Radius = area radius
	RainSystem(const physx::PxVec3 &origin, const double radius);
	~RainSystem();

	void init() override;
	void update(double dt) override;

protected:

	physx::PxVec3 _origin;
	double _radius;

	Particle* _modelParticle;
};

