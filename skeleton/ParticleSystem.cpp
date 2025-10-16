#include "ParticleSystem.h"


ParticleSystem::ParticleSystem()
{
}

ParticleSystem::~ParticleSystem()
{
	delete _modelParticle;
	_modelParticle = nullptr;

	for (auto& g : _generatorList) {
		if (g) {
			delete g;
			g = nullptr;
		}
	}

	for (auto& p : _particleList) {
		if (p) {
			delete p;
			p = nullptr;
		}
	}
}
