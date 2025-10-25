#include "RainSystem.h"

#include "UniformParticleGenerator.h"
#include "Particle.h"
#include "RainParticle.h"
#include "MeshData.h"


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

	_modelParticle = std::make_unique<RainParticle>(physx::PxTransform(_emitterOrigin), initDirection);
	_modelParticle->setVelocity(physx::PxVec3(0, -1, 0));

	// Create the generator
	std::unique_ptr<ParticleGenerator> generator = std::make_unique<UniformParticleGenerator>();

	generator->init(
		*_modelParticle,
		_emitterOrigin,
		Vector3Stats(physx::PxVec3(0.0f, -100.0f, 0.0f), physx::PxVec3(0.0f, -1.0f, 0.0f)) // velocity
	);

	// Create generation policy
	ParticleGenerationPolicy genPolicy = ParticleGenerationPolicy(
		true, ScalarStats(30.0, 1.0),
		true, ScalarStats(1.0, 1.0)
	);
	// Create the region shape for the policy
	// ParticleGenerationPolicy::volumeShape boxShape;
	// new (&boxShape.box) physx::PxBounds3(physx::PxVec3(-100.0f, -100.0f, -100.0f), physx::PxVec3(100.0f, 100.0f, 100.0f));
	//ParticleGenerationPolicy::volumeShape sphereShape;
	//new (&sphereShape.sphere) Vector3Stats(physx::PxVec3(0.0f, 0.0f, 0.0f), physx::PxVec3(10.0f, 10.0f, 10.0f));
	//ParticleGenerationPolicy::volumeShape pointShape;
	//new (&pointShape.point) Vector3Stats();

	// Set the region of generation: region type and shape
	// genPolicy.setRegion(SpawnRegionType::BOX, boxShape);
	//genPolicy.setRegion(SpawnRegionType::SPHERE, sphereShape);
	//genPolicy.setRegion(SpawnRegionType::POINT, pointShape);

	ParticleGenerationPolicy::volumeShape meshShape;
	MeshData meshData; 
	// meshData.loadMesh("../resources/cone.obj");
	meshData.loadMesh("../resources/monkey.obj");
	new (&meshShape.mesh) MeshData(meshData);
	genPolicy.setRegion(SpawnRegionType::MESH, meshShape);
	
	// Assign generator policy to the generator
	generator->setGenerationPolicy(genPolicy);


	// 
	//=========================================================================
	// Create lifetime policy
	ParticleLifetimePolicy lifePolicy = ParticleLifetimePolicy(
		ScalarStats(100, 0.1)
	);

	generator->setLifetimePolicy(lifePolicy);

	_generatorAndChildParticlesList.push_back(
		GeneratorAndChildParticles(
			std::move(generator),
			std::list<std::unique_ptr<Particle>>() // empty list
		)
	);

}

void RainSystem::update(double dt)
{
	//std::cout << "Updating rain system" << std::endl; // OK

	for (auto itG = _generatorAndChildParticlesList.begin(); itG != _generatorAndChildParticlesList.end(); )
	{
		//std::cout << "Updating rain system -> for each generator." << std::endl; // OK
		//std::cout << ">NUM OF PARTICLES: " << itG->second.size() << std::endl;

		// Existing particles
		for (auto itP = itG->second.begin(); itP != itG->second.end(); ) // for each particle of this generator father
		{
			//std::cout << "Updating rain system -> for each particle of this generator." << std::endl;

			(**itP).update(dt); // update particle

			if (mustDelete(**itP, *itG->first)) {
				itP = itG->second.erase(itP);  // erase returns the next valid iterator
			}
			else {
				++itP;  // only increment if we didnt erase
			}
		}

		//std::cout << "Before generating a particle" << std::endl; // OK
		// Particle generation => generate and add to list
		for (auto& p : (itG->first)->generateParticles(dt)) {
			//std::cout << "Generated a particle" << std::endl;
			itG->second.push_back(std::move(p));
		}

		++itG;
	}
}

void RainSystem::createParticleGenerator()
{
}
