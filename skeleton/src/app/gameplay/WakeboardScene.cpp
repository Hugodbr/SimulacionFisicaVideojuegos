#include "WakeboardScene.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/physics/PhysicsEngine.h"

#include "ForceManager.h"
#include "GravitationalForce.h"
#include "WindForce.h"

#include "RainSystem.h"
#include "SplashSystem.h"
#include "RopeSystem.h"

#include "EntityRenderable.h"

#include "WaterBody.h"
#include "BoxBody.h"
#include "SurfBoardBody.h"
#include "SurferBody.h"

#include "InputManager.h"
#include "GameApp.h"
#include "Camera.h"


WakeboardScene::~WakeboardScene()
{
}

void WakeboardScene::init()
{
	Scene::init();

    // =========================================================================================
	// Global Forces
	// =========================================================================================
	// Create and register a gravitational force for system of group ENVIRONMENT
	std::unique_ptr<ForceGenerator> gravForce = std::make_unique<GravitationalForce>();
	gravForce->setGroup(Constants::Group::DynamicGroup::ENVIRONMENT);
	ForceManager::getInstance().registerGlobalForce(std::move(gravForce));

	std::unique_ptr<ForceGenerator> windForce = std::make_unique<WindForce>(physx::PxVec3(20.0f, 0.0f, 0.0f));
	windForce->setGroup(Constants::Group::DynamicGroup::ENVIRONMENT);
	ForceManager::getInstance().registerGlobalForce(std::move(windForce));

    // =========================================================================================
    // Rigid Bodies
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
	// Surfer and Surfboard
	// =========================================================================================
	std::unique_ptr<RigidBody> surfer = std::make_unique<SurferBody>(
		physx::PxVec3(0.0f, 2.0f, 0.0f),
		ROOT_DIR + "\\resources\\glb\\silver_surfer.glb", 1.0f
	);
	surfer->init();

    physx::PxVec3 surferPosBottomY = physx::PxVec3(0.0f, 2.0f-(.0f + 0.74f/2.0f), 0.0f);
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
    _surferBoardMass = surfer->getMass() + board->getMass();

    // =========================================================================================
    // Boat
    // =========================================================================================
    physx::PxVec3 boatPos = physx::PxVec3(0.0f, 0.0f, 25.0f);
	std::unique_ptr<RigidBody> boat = std::make_unique<BoxBody>(
        boatPos,
		ROOT_DIR + "\\resources\\glb\\boat.glb",
        100.0f, // density
		0.025f
	);
	boat->init();
    _boatMass = boat->getMass();

	// =========================================================================================
    // Particle Systems
	// =========================================================================================
	// Splash System Surfer
	// =========================================================================================
	float halfRegionSizeSplash_X = board->getBounds().getExtents().x/1.5f;
	float halfRegionSizeSplash_Y = board->getBounds().getExtents().y/1.5f;
	float halfRegionSizeSplash_Z = board->getBounds().getExtents().z/1.5f;
	Region splashRegion(
		physx::PxBounds3(
			physx::PxVec3(-halfRegionSizeSplash_X, -halfRegionSizeSplash_Y, -halfRegionSizeSplash_Z), 
			physx::PxVec3(halfRegionSizeSplash_X, halfRegionSizeSplash_Y, halfRegionSizeSplash_Z)
		)
	);
	physx::PxVec3 splashOrigin = physx::PxVec3(0.0f, splashRegion.shape.box.minimum.y, 0.0f);
	
	std::unique_ptr<SplashSystem> splashSys = std::make_unique<SplashSystem>(splashOrigin, splashRegion);
	splashSys->init();
	splashSys->setRenderableEntity(std::make_unique<ModelSingleMeshPBR>(ROOT_DIR + "\\resources\\blender\\sphere.obj", 0.05f));
	splashSys->setGroups({ Constants::Group::DynamicGroup::ENVIRONMENT });
    splashSys->setCustomGenerationTriggerCallback([&]() {
        return !_surfBoardBody->isJumping();
    });
    splashSys->setGroups({ Constants::Group::DynamicGroup::ENVIRONMENT });

	_particleSystems.push_back(std::move(splashSys));
	_splashSystem = static_cast<SplashSystem*>(_particleSystems.back().get());
	PhysicsEngine::getInstance().pushParticleSystem(_particleSystems.back().get());

	// =========================================================================================
	// Splash System Boat
	// =========================================================================================
	halfRegionSizeSplash_X = boat->getBounds().getExtents().x;
	halfRegionSizeSplash_Y = boat->getBounds().getExtents().y;
	halfRegionSizeSplash_Z = boat->getBounds().getExtents().z;
	Region splashRegionBoat(
		physx::PxBounds3(
			physx::PxVec3(-halfRegionSizeSplash_X, -halfRegionSizeSplash_Y, -halfRegionSizeSplash_Z), 
			physx::PxVec3(halfRegionSizeSplash_X, halfRegionSizeSplash_Y, halfRegionSizeSplash_Z)
		)
	);
	splashOrigin = physx::PxVec3(0.0f, splashRegionBoat.shape.box.minimum.y, -boat->getBounds().getExtents().z/2.0f);
	
	std::unique_ptr<SplashSystem> splashSysBoat = std::make_unique<SplashSystem>(splashOrigin, splashRegionBoat);
	splashSysBoat->init();
	splashSysBoat->setRenderableEntity(std::make_unique<ModelSingleMeshPBR>(ROOT_DIR + "\\resources\\blender\\sphere.obj", 0.05f));
	splashSysBoat->setGroups({ Constants::Group::DynamicGroup::ENVIRONMENT });
    // splashSysBoat->setCustomGenerationTriggerCallback([&]() {
    //     return !_surfBoardBody->isJumping();
    // });
    splashSysBoat->setGroups({ Constants::Group::DynamicGroup::ENVIRONMENT });

	_particleSystems.push_back(std::move(splashSysBoat));
	_splashSystemBoat = static_cast<SplashSystem*>(_particleSystems.back().get());
	PhysicsEngine::getInstance().pushParticleSystem(_particleSystems.back().get());

	// =========================================================================================
    // Rigid Body Systems
    // =========================================================================================
    // Rope System
    // =========================================================================================
    physx::PxVec3 direction = boatPos - board->getBody()->getGlobalPose().p;
    float totalLength = direction.magnitude();

    physx::PxVec3 beginning = boatPos - physx::PxVec3(0.0f, 0.0f, boat->getDepth()/2.0f + 0.1f);
    physx::PxVec3 beginningAttachPoint = physx::PxVec3(0.0f, 0.0f, -(boat->getDepth()/2.0f + 1.5f));

    physx::PxVec3 end = board->getBody()->getGlobalPose().p + physx::PxVec3(0.0f, 0.0f, surfer->getDepth()/2.0f + 0.1f);
    physx::PxVec3 endAttachPoint = physx::PxVec3(0.0f, 0.0f, +(surfer->getDepth()/2.0f + 1.5f));

    std::unique_ptr<RigidBodySystem> ropeSystem = std::make_unique<RopeSystem>(
        beginning, // beginning
        boat.get(), //beginning body
        beginningAttachPoint, // beginning attach point

        end, // end
        surfer.get(), // end body
        endAttachPoint,  // end attach point

        35,   // number of rigid bodies
        2.0f, // body mass
        1000.0f, // spring constant k
        1.5f * totalLength,  // TODO max stretch
        ROOT_DIR + "\\resources\\blender\\sphere.obj",
        0.15f // scale
    );

    _rigidBodies.push_back(std::move(surfer));
	_surferBody = static_cast<SurferBody*>(_rigidBodies.back().get());
	PhysicsEngine::getInstance().pushRigidBody(_rigidBodies.back().get());

	_rigidBodies.push_back(std::move(board));
	_surfBoardBody = static_cast<SurfBoardBody*>(_rigidBodies.back().get());
	PhysicsEngine::getInstance().pushRigidBody(_rigidBodies.back().get());

    _rigidBodies.push_back(std::move(boat));
    _boat = static_cast<BoxBody*>(_rigidBodies.back().get());
	PhysicsEngine::getInstance().pushRigidBody(_rigidBodies.back().get());

    ropeSystem->init();
    _rigidBodySystems.push_back(std::move(ropeSystem));
    PhysicsEngine::getInstance().pushRigidBodySystem(_rigidBodySystems.back().get());

    setCamera();
}

void WakeboardScene::update()
{
    updateCamera();

    updateTraversal();
    updateSurfer();
    updateSplash();
	
	// ! physx is updating rigid bodies and particle systems at simulation step
	// TODO maybe remove from physx and update here
}

void WakeboardScene::updateTraversal()
{
    // if(InputManager::getInstance().isKeyPressedThisFrame(KeyCode::V))
    // {
    //     std::cout << "Applying impulse to surfer!" << std::endl;
    //     static_cast<physx::PxRigidBody*>(_surfBoardBody->getBody())->addForce(
    //         physx::PxVec3(0.0f, 0.0f, 100000.0f),
    //         physx::PxForceMode::eFORCE
    //     );
    // }

	// if(InputManager::getInstance().isKeyPressed(KeyCode::A))
    // {

    _traversalVelocity = physx::PxVec3(0.0f, 0.0f, 5.0f);

    if (static_cast<physx::PxRigidBody*>(_boat->getBody())->getLinearVelocity().z < 1.0f)
    {
        static_cast<physx::PxRigidBody*>(_boat->getBody())->addForce(
            _traversalVelocity,
            physx::PxForceMode::eVELOCITY_CHANGE
        );
    }
    
    if (static_cast<physx::PxRigidBody*>(_surferBody->getBody())->getLinearVelocity().z < 1.0f)
    {
        static_cast<physx::PxRigidBody*>(_surferBody->getBody())->addForce(
            _traversalVelocity * 0.8f,
            physx::PxForceMode::eVELOCITY_CHANGE
        );
    }
    
    if (static_cast<physx::PxRigidBody*>(_surfBoardBody->getBody())->getLinearVelocity().z < 1.0f)
    {
        static_cast<physx::PxRigidBody*>(_surfBoardBody->getBody())->addForce(
            _traversalVelocity * 0.8f,
            physx::PxForceMode::eVELOCITY_CHANGE
        );
    }
    // }


    // static_cast<physx::PxRigidBody*>(_surfBoardBody->getBody())->addForce(
    //     _traversalForcePerMass * _surferBoardMass,
    //     physx::PxForceMode::eFORCE
    // );

    // static_cast<physx::PxRigidBody*>(_boat->getBody())->addForce(
    //     _traversalForcePerMass * _boatMass,
    //     physx::PxForceMode::eFORCE
    // );

    // static_cast<physx::PxRigidBody*>(_surfBoardBody->getBody())->addForce(
    //     _traversalForcePerMass/5.0f,
    //     physx::PxForceMode::eACCELERATION
    // );

    // static_cast<physx::PxRigidBody*>(_boat->getBody())->addForce(
    //     _traversalForcePerMass/5.0f,
    //     physx::PxForceMode::eACCELERATION
    // );

    // static_cast<physx::PxRigidBody*>(_surferBody->getBody())->addForce(
    //     _traversalForcePerMass/5.0f,
    //     physx::PxForceMode::eACCELERATION
    // );



}

void WakeboardScene::updateSurfer()
{
    if(InputManager::getInstance().isKeyPressedThisFrame(KeyCode::Space) && !_surfBoardBody->isJumping())
    {
        std::cout << "Applying impulse to surfer!" << std::endl;
        static_cast<physx::PxRigidBody*>(_surferBody->getBody())->addForce(
            physx::PxVec3(0.0f, 40000.0f, 0.0f),
            physx::PxForceMode::eIMPULSE
        );
    }

    float torqueAmount = 10000.0f;
    float forceAmount = 5000.0f;

	if(InputManager::getInstance().isKeyPressedThisFrame(KeyCode::ArrowRight))
    {
        std::cout << "Applying TORQUE to surfer!" << std::endl;
        static_cast<physx::PxRigidBody*>(_surferBody->getBody())->addForce(
            -forceAmount * _surferBody->getDirectionRight(),
            physx::PxForceMode::eIMPULSE
        );
    }
	if(InputManager::getInstance().isKeyPressedThisFrame(KeyCode::ArrowLeft))
    {
        std::cout << "Applying TORQUE to surfer!" << std::endl;
        static_cast<physx::PxRigidBody*>(_surferBody->getBody())->addForce(
            forceAmount * _surferBody->getDirectionRight(),
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
}

void WakeboardScene::updateSplash()
{
    // std::cout << "vector size: " << _rigidBodies.size() << std::endl;
	_splashSystem->setEmitterOrigin(
		static_cast<physx::PxRigidBody*>(_surfBoardBody->getBody())->getGlobalPose().p
	);
    _splashSystemBoat->setEmitterOrigin(
        static_cast<physx::PxRigidBody*>(_boat->getBody())->getGlobalPose().p - physx::PxVec3(0.0f, 0.0f, _boat->getDepth()/2.0f)
    );
}

void WakeboardScene::setCamera()
{
    Camera& camera = dynamic_cast<GameApp&>(GameApp::getInstance()).getCamera();

    physx::PxVec3 eye = static_cast<physx::PxRigidBody*>(_surferBody->getBody())->getGlobalPose().p + physx::PxVec3(0.0f, 5.0f, -15.0f);
    physx::PxVec3 look = static_cast<physx::PxRigidBody*>(_surferBody->getBody())->getGlobalPose().p + physx::PxVec3(0.0f, 0.0f, 5.0f);
    physx::PxVec3 up = physx::PxVec3(0.0f, 1.0f, 0.0f);
    camera.setPositionParameters(eye, look, up);
}

void WakeboardScene::updateCamera()
{
    if(InputManager::getInstance().isKeyPressedThisFrame(KeyCode::C)) {
        freeCameraControl();
    }

    if (_freeCamera)
        return;

    setCamera();
}

void WakeboardScene::freeCameraControl()
{
    _freeCamera = !_freeCamera;
}
