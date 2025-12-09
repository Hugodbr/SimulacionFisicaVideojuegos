#include "Scene01.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/physics/PhysicsEngine.h"

#include "GravitationalForce.h"
#include "ForceManager.h"
#include "ExplosionForce.h"

#include "MeshSystem.h"
#include "RainSystem.h"

#include "EntityRenderable.h"

#include "WaterBody.h"
#include "BoxBody.h"


Scene01::~Scene01()
{
}

void Scene01::init()
{
	Scene::init();

	// =========================================================================================
	// Global Forces
	// =========================================================================================
	// Create and register a gravitational force for all particle systems
	std::unique_ptr<ForceGenerator> gravForce = std::make_unique<GravitationalForce>();
	gravForce->setGroup(Constants::Group::DynamicGroup::ENVIRONMENT);
	ForceManager::getInstance().registerGlobalForce(std::move(gravForce));

	// Explosion ==================================================================================
	// std::unique_ptr<ForceGenerator> explosionForceGen = std::make_unique<ExplosionForce>(
	// 	physx::PxVec3(0.0f, 0.0f, 0.0f), // explosion center
	// 	100000.0f,   // max radius
	// 	10000000.0f, // magnitude
	// 	3000000.0f // expansion velocity
	// );
	// explosionForceGen->setGroup(Constants::Group::DynamicGroup::ALL);
	// explosionForceGen->setTimer(5.0); // Delay activation by 5 seconds
	// // explosionForceGen->setExpireTime(20.0); // Expire after 15 seconds
	// explosionForceGen->setActive(false); // Initially inactive
	
	// std::cout << "ExplosionForce created." << std::endl;

	// ForceManager::getInstance().registerGlobalForce(std::move(explosionForceGen));

	// =========================================================================================
	// Cage System
	// =========================================================================================
	// std::unique_ptr<MeshSystem> meshSystem = std::make_unique<MeshSystem>(
	// 	projectRoot + "/resources/blender/originPoint.obj", 
	// 	3.0f, // point size
	// 	10.0, // scale
	// 	Constants::Color::Red // color
	// );
	// meshSystem->init();
	// meshSystem->setRenderableEntity(std::make_unique<ModelSingleMeshPBR>("C:\\Users\\hugod\\Github\\SimulacionFisicaVideojuegos\\resources\\fbx\\crate-box-free\\source\\Crate.fbx"));
	// meshSystem->setGroups({ Constants::Group::DynamicGroup::ENVIRONMENT });

	// _particleSystems.push_back(std::move(meshSystem));

	// PhysicsEngine::getInstance().pushParticleSystem(_particleSystems.back().get());

	// =========================================================================================
	// Rain System
	// =========================================================================================
	// float halfRegionSizeRain = 100.0f;
	// Region rainRegion(physx::PxBounds3(physx::PxVec3(-halfRegionSizeRain, -halfRegionSizeRain, -halfRegionSizeRain), physx::PxVec3(halfRegionSizeRain, halfRegionSizeRain, halfRegionSizeRain)));
	// physx::PxVec3 rainOrigin = physx::PxVec3(0.0f, rainRegion.shape.box.minimum.y, 0.0f);
	
	// std::unique_ptr<RainSystem> rs = std::make_unique<RainSystem>(rainOrigin, rainRegion);
	// rs->init();
	// rs->setRenderableEntity(std::make_unique<ModelSingleMeshPBR>("C:\\Users\\hugod\\Github\\SimulacionFisicaVideojuegos\\resources\\fbx\\crate-box-free\\source\\Crate.fbx"));
	// rs->setGroups({ Constants::Group::DynamicGroup::ENVIRONMENT, Constants::Group::DynamicGroup::ENEMY });

	// _particleSystems.push_back(std::move(rs));

	// PhysicsEngine::getInstance().pushParticleSystem(_particleSystems.back().get());

	// =========================================================================================
	// Water Body
	// =========================================================================================
	std::unique_ptr<RigidBody> waterBody = std::make_unique<WaterBody>(
		physx::PxVec3(0, 0, 0),
		ROOT_DIR + "\\resources\\blender\\water.glb"
	);
	waterBody->init();
	// waterBody->setRenderableEntity(std::make_unique<ModelSingleMeshPBR>("C:\\Users\\hugod\\Github\\SimulacionFisicaVideojuegos\\resources\\blender\\water.glb")
	// );
	_rigidBodies.push_back(std::move(waterBody));
	PhysicsEngine::getInstance().pushRigidBody(_rigidBodies.back().get());

	// =========================================================================================
	// RigidBody Box
	// =========================================================================================
	std::unique_ptr<RigidBody> boxBody = std::make_unique<BoxBody>(
		physx::PxVec3(0.0f, 5.0f, 0.0f),
		ROOT_DIR + "\\resources\\fbx\\crate-box-free\\source\\Crate.fbx"
	);
	boxBody->init();
	_rigidBodies.push_back(std::move(boxBody));
	PhysicsEngine::getInstance().pushRigidBody(_rigidBodies.back().get());


	// =========================================================================================

    // Material copper_material(glm::vec4(1, 1, 0, 1)); // needs to call setCopper() to initialize parameters. These parameters are used to set the uniforms at th render of the entity when called mMaterial.upload
	// copper_material.setCopper();
	// Sphere* copperSphere = new Sphere(50, 20, 30);
	// copperSphere->setWPos(glm::vec3(150, 0, 0));
	// copperSphere->setMaterial(copper_material);
	// gObjects.push_back(copperSphere);
}

void Scene01::update()
{
	// ! physx is updating rigid bodies and particle systems at simulation step
	// TODO maybe remove from physx and update here
}
