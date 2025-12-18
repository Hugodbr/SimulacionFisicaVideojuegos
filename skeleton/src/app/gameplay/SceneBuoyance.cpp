#include "SceneBuoyance.h"

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


SceneBuoyance::~SceneBuoyance()
{
}

void SceneBuoyance::init()
{
	Scene::init();

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
		physx::PxVec3(0.0f, 2.0f, 0.0f),
		ROOT_DIR + "\\resources\\fbx\\crate-box-free\\source\\Crate.fbx",	
		// ROOT_DIR + "\\resources\\blender\\wooden_boat.glb",
		// 0.025f
		500.0f,
		1.0f
		// ROOT_DIR + "\\resources\\glb\\free_stalker_barrel_pack_-_low-poly_props.glb"
		// ROOT_DIR + "\\resources\\blender\\sphere.obj"
		// ROOT_DIR + "\\resources\\glb\\klk-17_pistol.obj"
		// ROOT_DIR + "\\resources\\glb\\surfboard__tabla_de_surf.glb", 0.05f
		// ROOT_DIR + "\\resources\\glb\\silver_surfer.glb", 1.0f, 1.0f
	);
	boxBody->init();
    // boxBody->setDensity(800.0f);
	_rigidBodies.push_back(std::move(boxBody));
	PhysicsEngine::getInstance().pushRigidBody(_rigidBodies.back().get());
}

void SceneBuoyance::update()
{
	// ! physx is updating rigid bodies and particle systems at simulation step
	// TODO maybe remove from physx and update here
}
