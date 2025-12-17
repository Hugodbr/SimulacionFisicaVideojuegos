#include "SceneSurfer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/physics/PhysicsEngine.h"

#include "GravitationalForce.h"
#include "ForceManager.h"

#include "MeshSystem.h"
#include "RainSystem.h"
#include "SplashSystem.h"

#include "EntityRenderable.h"

#include "WaterBody.h"
#include "SurfBoardBody.h"
#include "SurferBody.h"

#include "InputManager.h"


SceneSurfer::~SceneSurfer()
{
}

void SceneSurfer::init()
{
	Scene::init();

	// =========================================================================================
	// Water Body
	// =========================================================================================
	std::unique_ptr<RigidBody> waterBody = std::make_unique<WaterBody>(
		physx::PxVec3(0, 0, 0),
		ROOT_DIR + "\\resources\\glb\\water.glb"
	);
	waterBody->init();
	_rigidBodies.push_back(std::move(waterBody));
	PhysicsEngine::getInstance().pushRigidBody(_rigidBodies.back().get());

	// =========================================================================================
	// RigidBody Box
	// =========================================================================================
	std::unique_ptr<RigidBody> surfer = std::make_unique<SurferBody>(
		physx::PxVec3(0.0f, 5.0f, 0.0f),
		ROOT_DIR + "\\resources\\glb\\silver_surfer.glb", 1.0f
	);
	surfer->init();

    physx::PxVec3 surferPosBottomY = physx::PxVec3(0.0f, 5.0f-(.0f + 0.74f/2.0f), 0.0f);
    // physx::PxVec3 surferPosBottomY = physx::PxVec3(0.0f, 5.0f-(surfer->getHeight()/2.0f + 0.74f/2.0f), 0.0f);
	std::unique_ptr<RigidBody> board = std::make_unique<SurfBoardBody>(
		// physx::PxVec3(0.0f, surfer->getBounds().minimum.y + 0.1f, 0.0f),
		surferPosBottomY,
		// ROOT_DIR + "\\resources\\glb\\surfboard__tabla_de_surf.glb", 0.05f
		ROOT_DIR + "\\resources\\glb\\tabla1.glb", 0.04f
	);
	board->init();


    physx::PxD6Joint* joint = physx::PxD6JointCreate(
        *PhysicsEngine::getInstance().getPhysics(),
        surfer->getBody(),
        physx::PxTransform(physx::PxVec3(0.0f, -surfer->getHeight()/2.0f + 0.4f, 0.0f)),
        board->getBody(),
        physx::PxTransform(physx::PxVec3(0.0f, board->getHeight()/2.0f, 0.0f))
    ); 
 /*
    // joint->setMotion(physx::PxD6Axis::eX, physx::PxD6Motion::eLOCKED);
    // joint->setMotion(physx::PxD6Axis::eY, physx::PxD6Motion::eLOCKED);
    // joint->setMotion(physx::PxD6Axis::eZ, physx::PxD6Motion::eLOCKED);

    // joint->setMotion(physx::PxD6Axis::eTWIST, physx::PxD6Motion::eLOCKED);
    // joint->setMotion(physx::PxD6Axis::eSWING1, physx::PxD6Motion::eLOCKED);
    // joint->setMotion(physx::PxD6Axis::eSWING2, physx::PxD6Motion::eLOCKED);
    // joint->setConstraintFlag(physx::PxConstraintFlag::ePROJECTION, true);

    // joint->setProjectionLinearTolerance(0.01f);
    // joint->setProjectionAngularTolerance(0.01f);
*/
	_rigidBodies.push_back(std::move(surfer));
	_surferBody = static_cast<SurferBody*>(_rigidBodies.back().get());
	PhysicsEngine::getInstance().pushRigidBody(_rigidBodies.back().get());

	_rigidBodies.push_back(std::move(board));
	_surfBoardBody = static_cast<SurfBoardBody*>(_rigidBodies.back().get());
	PhysicsEngine::getInstance().pushRigidBody(_rigidBodies.back().get());

	// =========================================================================================
	// =========================================================================================
	// Splash System
	// =========================================================================================
	float halfRegionSizeSplash_X = _surfBoardBody->getBounds().getExtents().x/1.5f;
	float halfRegionSizeSplash_Y = _surfBoardBody->getBounds().getExtents().y/1.5f;
	float halfRegionSizeSplash_Z = _surfBoardBody->getBounds().getExtents().z/1.5f;
	Region splashRegion(
		physx::PxBounds3(
			physx::PxVec3(-halfRegionSizeSplash_X, -halfRegionSizeSplash_Y, -halfRegionSizeSplash_Z), 
			physx::PxVec3(halfRegionSizeSplash_X, halfRegionSizeSplash_Y, halfRegionSizeSplash_Z)
		)
	);
	physx::PxVec3 splashOrigin = physx::PxVec3(0.0f, splashRegion.shape.box.minimum.y, 0.0f);
	
	std::unique_ptr<SplashSystem> splashSys = std::make_unique<SplashSystem>(splashOrigin, splashRegion);
	splashSys->init();
	splashSys->setRenderableEntity(std::make_unique<ModelSingleMeshPBR>("C:\\Users\\hugod\\Github\\SimulacionFisicaVideojuegos\\resources\\glb\\low_poly_water_drop.glb",0.4f));
	splashSys->setGroups({ Constants::Group::DynamicGroup::ENVIRONMENT });

	_particleSystems.push_back(std::move(splashSys));
	_splashSystem = static_cast<SplashSystem*>(_particleSystems.back().get());
	PhysicsEngine::getInstance().pushParticleSystem(_particleSystems.back().get());
}

void SceneSurfer::update()
{
    // std::cout << "vector size: " << _rigidBodies.size() << std::endl;
	_splashSystem->setEmitterOrigin(
		static_cast<physx::PxRigidBody*>(_surfBoardBody->getBody())->getGlobalPose().p
	);

	if (_surfBoardBody->isJumping()) {
		std::cout << "Surfer is jumping, disabling splash system." << std::endl;
	    _splashSystem->setActive(false);
	}
	else {
		std::cout << "Surfer is NOT jumping, enabling splash system." << std::endl;
	    _splashSystem->setActive(true);
	}


    if(InputManager::getInstance().isKeyPressedThisFrame(KeyCode::Space) && !_surfBoardBody->isJumping())
    {
        std::cout << "Applying impulse to surfer!" << std::endl;
        static_cast<physx::PxRigidBody*>(_rigidBodies[1].get()->getBody())->addForce(
            physx::PxVec3(0.0f, 20000.0f, 0.0f),
            physx::PxForceMode::eIMPULSE
        );
    }

	if(InputManager::getInstance().isKeyPressedThisFrame(KeyCode::V))
    {
        std::cout << "Applying impulse to surfer!" << std::endl;
        static_cast<physx::PxRigidBody*>(_surfBoardBody->getBody())->addForce(
            physx::PxVec3(0.0f, 0.0f, 100000.0f),
            physx::PxForceMode::eFORCE
        );
    }
	
	float torqueAmount = 100000.0f;
	if(InputManager::getInstance().isKeyPressedThisFrame(KeyCode::ArrowRight))
    {
        std::cout << "Applying TORQUE to surfer!" << std::endl;
        static_cast<physx::PxRigidBody*>(_surferBody->getBody())->addTorque(
            torqueAmount * _surferBody->getDirectionForward(),
            physx::PxForceMode::eIMPULSE
        );
    }
	if(InputManager::getInstance().isKeyPressedThisFrame(KeyCode::ArrowLeft))
    {
        std::cout << "Applying TORQUE to surfer!" << std::endl;
        static_cast<physx::PxRigidBody*>(_surferBody->getBody())->addTorque(
            -torqueAmount * _surferBody->getDirectionForward(),
            physx::PxForceMode::eIMPULSE
        );
    }

	if(InputManager::getInstance().isKeyPressedThisFrame(KeyCode::ArrowUp))
    {
        std::cout << "Applying TORQUE to surfer!" << std::endl;
        static_cast<physx::PxRigidBody*>(_surferBody->getBody())->addTorque(
            torqueAmount * _surferBody->getDirectionRight(),
            physx::PxForceMode::eIMPULSE
        );
    }
	if(InputManager::getInstance().isKeyPressedThisFrame(KeyCode::ArrowDown))
    {
        std::cout << "Applying TORQUE to surfer!" << std::endl;
        static_cast<physx::PxRigidBody*>(_surferBody->getBody())->addTorque(
            -torqueAmount * _surferBody->getDirectionRight(),
            physx::PxForceMode::eIMPULSE
        );
    }

	// ! physx is updating rigid bodies and particle systems at simulation step
	// TODO maybe remove from physx and update here
}
