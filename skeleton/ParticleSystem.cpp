#include "ParticleSystem.h"


ParticleSystem::ParticleSystem()
{
}

ParticleSystem::~ParticleSystem()
{
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
