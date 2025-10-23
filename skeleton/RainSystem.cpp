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
	// Create the model particle
	physx::PxTransform initTransform = physx::PxTransform(0, 0, 0);
	physx::PxVec3 initDirection = physx::PxVec3(0, -1, 0);

	_modelParticle = std::make_unique<RainParticle>(initTransform, initDirection);

	// Create the generator
	std::unique_ptr<ParticleGenerator> generator = std::make_unique<UniformParticleGenerator>();

	generator->init(
		_emitterOrigin,
		Vector3Stats(physx::PxVec3(0.0f, -10.0f, 0.0f), physx::PxVec3(0.0f, -1.0f, 0.0f)),
		*_modelParticle
	);

	// Create generation policy
	ParticleGenerationPolicy genPolicy = ParticleGenerationPolicy(
		true, ScalarStats(1.0, 1.0),
		true, ScalarStats(1000.0, 1.0)
	);
	// Create the region shape for the policy
	ParticleGenerationPolicy::volumeShape pointShape = ParticleGenerationPolicy::volumeShape::volumeShape();
	pointShape.point = Vector3Stats();
	// Set the region of generation: region type and shape
	genPolicy.setRegion(SpawnRegionType::POINT, pointShape);
	// Assignate generator policy to the generator
	generator->setGenerationPolicy(genPolicy);

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
