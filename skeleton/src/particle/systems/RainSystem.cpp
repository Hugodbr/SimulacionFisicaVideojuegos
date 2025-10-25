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

	// Create particle generators
	createParticleGenerators();
}

void RainSystem::update(double deltaTime)
{
	for (auto itG = _generatorAndChildParticlesList.begin(); itG != _generatorAndChildParticlesList.end(); ++itG)
	{
		// Existing particles
		for (auto& p : itG->second) { // for each particle of this generator father
			p->update(deltaTime); // update particle
		}
		// Particle generation => generate and add to list
		for (auto& p : (itG->first)->generateParticles(deltaTime)) {
			itG->second.push_back(std::move(p));
		}
		// Cleanup dead particles
		removeDeadParticles(itG);
	}
}

uint64_t RainSystem::getReserveCountPerGenerator() const
{
    return Constants::System::Rain::ReserveCountPerGenerator;
}

void RainSystem::createParticleGenerators()
{
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
	meshData.loadMesh("../resources/circle.obj");
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

	// Register the generator to this system
	registerParticleGenerator(generator);
}
