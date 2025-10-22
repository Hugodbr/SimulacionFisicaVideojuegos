#include "RainSystem.h"

#include "UniformParticleGenerator.h"
#include "Particle.h"
#include "RainParticle.h"


RainSystem::RainSystem(const physx::PxVec3& origin)
	: ParticleSystem()
{
	_emitterOrigin = origin;
}

void RainSystem::init()
{
	// Create generator and push back to _generators
	//_particleList.push_back(std::make_unique<Particle>(/* args */));
	//_generatorList.push_back(std::make_unique<ParticleGenerator>(/* args */));

	_modelParticle = std::make_unique<RainParticle>(/* args */);

	std::unique_ptr<ParticleGenerator> generator = std::make_unique<UniformParticleGenerator>();

	generator->init(
		_emitterOrigin,
		Vector3Stats(physx::PxVec3(0.0f, -10.0f, 0.0f), physx::PxVec3(0.0f, -1.0f, 0.0f)),
		*_modelParticle
	);

	_generatorAndChildParticlesList.push_back(
		GeneratorAndChildParticles(
			std::move(generator),
			std::list<std::unique_ptr<Particle>>() // empty list
		)
	);

}

void RainSystem::update(double dt)
{
	for (auto itG = _generatorAndChildParticlesList.begin(); itG != _generatorAndChildParticlesList.end(); )
	{
		// Existing particles
		for (auto itP = itG->second.begin(); itP != itG->second.end(); ) // for each particle of this generator father
		{
			(**itP).update(dt); // update particle

			if (mustDelete(**itP, *itG->first)) { // check if must delete particle according to generator policy
				itG->second.erase(itP);
			}
			++itP;
		}

		// Particle generation => generate and add to list
		for (auto& p : (itG->first)->generateParticles(dt)) {
			itG->second.push_back(std::move(p));
		}

		++itG;
	}
}
