#include "RainSystem.h"

#include "UniformParticleGenerator.h"


RainSystem::RainSystem(const physx::PxVec3& origin, const double radius)
	: ParticleSystem(),
	_origin(origin),
	_radius(radius)
{
}

RainSystem::~RainSystem()
{
}

void RainSystem::init()
{
	// Create generator and push bakc to _generators
	UniformParticleGenerator* ug = new UniformParticleGenerator()

	ug->setOrigin(_origin)


	_generatorList.push_back(ug);
}

void RainSystem::update(double dt)
{
}
