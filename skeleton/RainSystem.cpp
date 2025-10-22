#include "RainSystem.h"

#include "UniformParticleGenerator.h"
#include "Particle.h"
#include "RainParticle.h"


RainSystem::RainSystem(const physx::PxVec3& origin, const double radius)
	: ParticleSystem()
{
	_emitterOrigin = origin;
	_radius = radius;

	//_modelParticle = new RainParticle()
}

RainSystem::~RainSystem()
{
	_particleList.clear();
	_generatorList.clear();
}

void RainSystem::init()
{
	// Create generator and push bakc to _generators
	UniformParticleGenerator* ug = new UniformParticleGenerator();

	ug->setModelParticle(_modelParticle);

	_generatorList.push_back(ug);
}

void RainSystem::update(double dt)
{
	for (auto& generator : _generatorList) {
		for (auto& newParticle : generator->generateParticles(dt)) {
			_particleList.push_back(newParticle);
		}
	}

	for (auto& particle : _particleList) {
		particle->update(dt);

	}

	for (auto& particle : _particleList) {
		if (!particle->isAlive()) {
			_particleList.remove(particle);
		}
	}
}
