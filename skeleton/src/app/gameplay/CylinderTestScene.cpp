#include "CylinderTestScene.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/physics/PhysicsEngine.h"

#include "GravitationalForce.h"
#include "ForceManager.h"
#include "ExplosionForce.h"

#include "MeshSystem.h"
#include "RainSystem.h"

#include "EntityRenderable.h"

#include "FloorBody.h"
#include "BarrelBody.h"
#include "BoxBody.h"


CylinderTestScene::~CylinderTestScene()
{
}

void CylinderTestScene::init()
{
	Scene::init();

	// =========================================================================================
	// Floor Body
	// =========================================================================================
	std::unique_ptr<RigidBody> floorBody = std::make_unique<FloorBody>(
		physx::PxVec3(0, 0, 0),
		ROOT_DIR + "\\resources\\blender\\water.glb",
        physx::PxQuat(0.0f, physx::PxVec3(0.0f, 1.0f, 0.0f)),
        1.0f
	);
	floorBody->init();
	// floorBody->setRenderableEntity(std::make_unique<ModelSingleMeshPBR>("C:\\Users\\hugod\\Github\\SimulacionFisicaVideojuegos\\resources\\blender\\water.glb")
	// );
	_rigidBodies.push_back(std::move(floorBody));
	PhysicsEngine::getInstance().pushRigidBody(_rigidBodies.back().get());

	// =========================================================================================
	// RigidBody Box
	// =========================================================================================
	std::unique_ptr<RigidBody> barrelBody = std::make_unique<BarrelBody>(
		physx::PxVec3(0.0f, 3.0f, 0.0f),
		// ROOT_DIR + "\\resources\\fbx\\crate-box-free\\source\\Crate.fbx",	
		// ROOT_DIR + "\\resources\\blender\\wooden_boat.glb",
		// 0.025f
		ROOT_DIR + "\\resources\\glb\\free_stalker_barrel_pack_-_low-poly_props.glb",
		500.0f,
		1.0f
		// ROOT_DIR + "\\resources\\blender\\sphere.obj"
		// ROOT_DIR + "\\resources\\glb\\klk-17_pistol.obj"
		// ROOT_DIR + "\\resources\\glb\\surfboard__tabla_de_surf.glb", 0.05f
		// ROOT_DIR + "\\resources\\glb\\silver_surfer.glb", 1.0f, 1.0f
	);
	barrelBody->init();
    // barrelBody->setDensity(800.0f);
	_rigidBodies.push_back(std::move(barrelBody));
	PhysicsEngine::getInstance().pushRigidBody(_rigidBodies.back().get());

	// =========================================================================================
	// RigidBody Box
	// =========================================================================================
	std::unique_ptr<RigidBody> boxBody = std::make_unique<BoxBody>(
		physx::PxVec3(10.0f, 10.0f, 10.0f),
		ROOT_DIR + "\\resources\\fbx\\crate-box-free\\source\\Crate.fbx",	
		// ROOT_DIR + "\\resources\\blender\\wooden_boat.glb",
		// 0.025f   
		// ROOT_DIR + "\\resources\\glb\\free_stalker_barrel_pack_-_low-poly_props.glb",
		500.0f,
		1.0f
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

void CylinderTestScene::update()
{
	// ! physx is updating rigid bodies and particle systems at simulation step
	// TODO maybe remove from physx and update here
}
