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
	physx::PxTransform initTransform = physx::PxTransform(_emitterOrigin);
	physx::PxVec3 initDirection = physx::PxVec3(0, -1, 0);

	//_modelParticle = std::make_shared<RainParticle>(initTransform, initDirection);
	_modelParticle = new RainParticle(initTransform, initDirection);
	_modelParticle->setVelocity(physx::PxVec3(0, -1, 0));
	//_modelParticle->setColor(Constants::Color::Green);
	//_modelParticle->setOrigin(physx::PxTransform(-10, 60, 10));
	//_modelParticle->removeRenderItem();

	// Create the generator
	//std::unique_ptr<ParticleGenerator> generator = std::make_unique<UniformParticleGenerator>();
	ParticleGenerator* generator = new UniformParticleGenerator();

	generator->init(
		_emitterOrigin,
		Vector3Stats(physx::PxVec3(0.0f, -100.0f, 0.0f), physx::PxVec3(0.0f, -1.0f, 0.0f)), // velocity
		_modelParticle
	);

	//=========================================================================
	// Create generation policy
	ParticleGenerationPolicy genPolicy = ParticleGenerationPolicy(
		true, ScalarStats(1.0, 1.0),
		true, ScalarStats(1.0, 1.0)
	);
	// Create the region shape for the policy
	//ParticleGenerationPolicy::volumeShape sphereShape;
	//new (&sphereShape.sphere) Vector3Stats(physx::PxVec3(0.0f, 0.0f, 0.0f), physx::PxVec3(100.0f, 100.0f, 100.0f));
	ParticleGenerationPolicy::volumeShape pointShape;
	new (&pointShape.point) Vector3Stats(physx::PxVec3(0.0f, 50.0f, 0.0f), physx::PxVec3(0, 0, 0));
	//new (&pointShape.point) Vector3Stats(physx::PxVec3(0.0f, -100.0f, 0.0f)_emitterOrigin);

	// Set the region of generation: region type and shape
	//genPolicy.setRegion(SpawnRegionType::SPHERE, sphereShape);
	genPolicy.setRegion(SpawnRegionType::POINT, pointShape);

	// Assign generator policy to the generator
	generator->setGenerationPolicy(genPolicy);

	//=========================================================================
	// Create lifetime policy
	ParticleLifetimePolicy lifePolicy = ParticleLifetimePolicy(
		ScalarStats(5, 0.1)
	);

	generator->setLifetimePolicy(lifePolicy);

	//=========================================================================
	//_generatorAndChildParticlesList.push_back(
	//	GeneratorAndChildParticles(
	//		std::move(generator),
	//		std::list<std::unique_ptr<Particle>>() // empty list
	//	)
	//);
	_generatorAndChildParticlesList.push_back(
		GeneratorAndChildParticles(
			generator,
			std::vector<Particle*>() // empty list
		)
	);
	std::cout << "GenList: " << std::endl;

}

//void RainSystem::update(double dt)
//{
//	//std::cout << "Updating rain system" << std::endl; // OK
//
//	for (auto itG = _generatorAndChildParticlesList.begin(); itG != _generatorAndChildParticlesList.end(); )
//	{
//		//std::cout << "Updating rain system -> for each generator." << std::endl; // OK
//		//std::cout << ">NUM OF PARTICLES: " << itG->second.size() << std::endl;
//
//		// Existing particles
//		for (auto itP = itG->second.begin(); itP != itG->second.end(); ) // for each particle of this generator father
//		{
//			//std::cout << "Updating rain system -> for each particle of this generator." << std::endl;
//
//			(**itP).update(dt); // update particle
//
//			if (mustDelete(**itP, *itG->first)) { // check if must delete particle according to generator policy
//				itP = itG->second.erase(itP);
//			}
//			else {
//				++itP;
//			}
//		}
//
//		//std::cout << "Before generating a particle" << std::endl; // OK
//		
//		//// Particle generation => generate and add to list
//		for (std::unique_ptr<Particle>& p : (itG->first)->generateParticles(dt)) {
//			//std::cout << "Generated a particle" << std::endl;
//			itG->second.push_back(std::move(p));
//		}
//
//		++itG;
//	}
//}

//void RainSystem::update(double dt)
//{
//	//std::cout << "Updating rain system" << std::endl; // OK
//
//	for (auto itG = _generatorAndChildParticlesList.begin(); itG != _generatorAndChildParticlesList.end(); )
//	{
//		//std::cout << "Updating rain system -> for each generator." << std::endl; // OK
//		//std::cout << ">NUM OF PARTICLES: " << itG->second.size() << std::endl;
//
//		// Existing particles
//		for (auto itP = itG->second.begin(); itP != itG->second.end(); ) // for each particle of this generator father
//		{
//			std::cout << "Updating rain system -> for each particle of this generator." << std::endl;
//
//			(**itP).update(dt); // update particle
//
//			if (mustDelete(**itP, *itG->first)) { // check if must delete particle according to generator policy
//				itP = itG->second.erase(itP);
//			}
//			else {
//				++itP;
//			}
//		}
//
//		//std::cout << "Before generating a particle" << std::endl; // OK
//		
//		//// Particle generation => generate and add to list
//		for (auto& p : (itG->first)->generateParticles(dt)) {
//			//std::cout << "Generated a particle" << std::endl;
//			itG->second.push_back(p);
//		}
//
//		++itG;
//	}
//}

void RainSystem::update(double dt)
{
	//std::cout << "Updating rain system" << std::endl; // OK

	for (int i = 0; i < _generatorAndChildParticlesList.size(); ++i)
	{
		//std::cout << "Updating rain system -> for each generator." << std::endl; // OK
		//std::cout << ">NUM OF PARTICLES: " << itG->second.size() << std::endl;

		// Existing particles
		auto& particles = _generatorAndChildParticlesList[i].second;
		auto& gen = _generatorAndChildParticlesList[i].first;

		for (int j = 0; j < particles.size(); ++j) // for each particle of this generator father
		{
			std::cout << "Updating rain system -> for each particle of this generator." << std::endl;

			particles[j]->update(dt); // update particle

			if (mustDelete(*particles[j], *gen)) { // check if must delete particle according to generator policy
				particles[j]->kill();
			}
		}

		particles.erase(std::remove_if(
				particles.begin(),
				particles.end(),
				[](Particle* p) {
					return !p->isAlive();
				}
			),
			particles.end()
		);

		//std::cout << "Before generating a particle" << std::endl; // OK

		//// Particle generation => generate and add to list
		for (auto* p : gen->generateParticles(dt)) {
			//std::cout << "Generated a particle" << std::endl;
			particles.push_back(p);
		}

	}
}

void RainSystem::doit()
{
	//std::cout << "DOOOIIIITTTT" << std::endl;
	////auto& gen = _generatorAndChildParticlesList.begin()->first;
	////auto& list = _generatorAndChildParticlesList.begin()->second;
	////
	////list.push_back(std::move(gen->generateParticle()));

	//physx::PxTransform initTransform = physx::PxTransform(0, 0, 0);
	//physx::PxVec3 initDirection = physx::PxVec3(0, -1, 0);

	//_p.push_back(new RainParticle(initTransform, initDirection));
}
