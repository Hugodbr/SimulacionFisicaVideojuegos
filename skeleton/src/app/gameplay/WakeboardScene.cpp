#include "WakeboardScene.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/physics/PhysicsEngine.h"

#include "ForceManager.h"
#include "GravitationalForce.h"
#include "WindForce.h"

#include "Stats.h"

#include "RainSystem.h"
#include "SplashSystem.h"
#include "RopeSystem.h"
#include "TrashSystem.h"

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

    surfer_board_joint = physx::PxD6JointCreate(
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

	_particleSystems.push_back(std::move(splashSysBoat));
	_splashSystemBoat = static_cast<SplashSystem*>(_particleSystems.back().get());
	PhysicsEngine::getInstance().pushParticleSystem(_particleSystems.back().get());

    // =========================================================================================
	// Rain System
	// =========================================================================================
    // float halfRegionSizeRain = 40.0f;
	// Region rainRegion(physx::PxBounds3(physx::PxVec3(-halfRegionSizeRain, -halfRegionSizeRain, -halfRegionSizeRain), physx::PxVec3(halfRegionSizeRain, halfRegionSizeRain, halfRegionSizeRain)));
	// physx::PxVec3 rainOrigin = physx::PxVec3(0.0f, rainRegion.shape.box.maximum.y, 0.0f);
	
	// std::unique_ptr<RainSystem> rs = std::make_unique<RainSystem>(rainOrigin, rainRegion);
	// rs->init();
    // rs->setRenderableEntity(std::make_unique<ModelSingleMeshPBR>("C:\\Users\\hugod\\Github\\SimulacionFisicaVideojuegos\\resources\\glb\\low_poly_water_drop.glb", 0.1f));
	// rs->setGroups({ Constants::Group::DynamicGroup::ENVIRONMENT });
    // rs->setEmissionRate(10);

	// _particleSystems.push_back(std::move(rs));
    // _rainSystem = static_cast<RainSystem*>(_particleSystems.back().get());
	// PhysicsEngine::getInstance().pushParticleSystem(_particleSystems.back().get());

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

        40,   // number of rigid bodies
        2.0f, // body mass
        1000.0f, // spring constant k
        1.5f * totalLength, 
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

    // Surfer and board with joint rewgister
    _surferBody->setJoint(surfer_board_joint);

    // =========================================================================================
    // Trash System
    // =========================================================================================
    // float halfRegionSizeTrash_X = 10.0f;
	// float halfRegionSizeTrash_Y = 0.1f;
	// float halfRegionSizeTrash_Z = 10.0f;
	// Region trashRegion(
	// 	physx::PxBounds3(
	// 		physx::PxVec3(-halfRegionSizeTrash_X, -halfRegionSizeTrash_Y, -halfRegionSizeTrash_Z), 
	// 		physx::PxVec3(halfRegionSizeTrash_X, halfRegionSizeTrash_Y, halfRegionSizeTrash_Z)
	// 	)
	// );

    // physx::PxVec3 trashOrigin = _surfBoardBody->getPosition();
    // std::unique_ptr<RigidBodySystem> trashSystem = std::make_unique<TrashSystem>(
    //     trashOrigin,
    //     trashRegion,
    //     ROOT_DIR + "\\resources\\fbx\\crate-box-free\\source\\Crate.fbx",
    //     1.0f
    // );
    // trashSystem->init();

    // static_cast<TrashSystem*>(trashSystem.get())->setFollowTarget(*_boat);
    // _rigidBodySystems.push_back(std::move(trashSystem));
    // PhysicsEngine::getInstance().pushRigidBodySystem(_rigidBodySystems.back().get());

    // =========================================================================================
    // Set initial camera position and orientation
    // =========================================================================================
    setCamera();

    _lastEye = static_cast<physx::PxRigidBody*>(_surferBody->getBody())->getGlobalPose().p + physx::PxVec3(0.0f, 5.0f, -15.0f);

    _traversalVelocity = physx::PxVec3(0.0f, 0.0f, 5.0f);
    _lastTraversalVelocity = _traversalVelocity;

    _splashSystem->setEmissionRate(_splashBaseEmissionRate_Surfer);
    _splashSystemBoat->setEmissionRate(_splashBaseEmissionRate_Boat);
}

void WakeboardScene::update()
{
    updateCamera();

    updateTraversal();
    updateSurfer();
    updateSplash();
    updateRain();
	
	// ! physx is updating rigid bodies and particle systems at simulation step
}

void WakeboardScene::updateTraversal()
{
    if (InputManager::getInstance().isKeyPressedThisFrame(KeyCode::W))
    {
        _traversalVelocity.z += 1.0f;
        // std::cout << "Increasing traversal velocity to " << _traversalVelocity.z << std::endl;
    }
    else if (InputManager::getInstance().isKeyPressedThisFrame(KeyCode::S))
    {
        _traversalVelocity.z -= 1.0f;
    }

    if (static_cast<physx::PxRigidBody*>(_boat->getBody())->getLinearVelocity().z < abs(_traversalVelocity.z))
    {
        static_cast<physx::PxRigidBody*>(_boat->getBody())->addForce(
            _traversalVelocity,
            physx::PxForceMode::eVELOCITY_CHANGE
        );
    }
    
    if (static_cast<physx::PxRigidBody*>(_surferBody->getBody())->getLinearVelocity().z < abs(_traversalVelocity.z))
    {
        static_cast<physx::PxRigidBody*>(_surferBody->getBody())->addForce(
            _traversalVelocity * 0.8f,
            physx::PxForceMode::eVELOCITY_CHANGE
        );
    }
    
    if (static_cast<physx::PxRigidBody*>(_surfBoardBody->getBody())->getLinearVelocity().z < abs(_traversalVelocity.z))
    {
        static_cast<physx::PxRigidBody*>(_surfBoardBody->getBody())->addForce(
            _traversalVelocity * 0.8f,
            physx::PxForceMode::eVELOCITY_CHANGE
        );
    }
}

void WakeboardScene::updateSurfer()
{
    if(InputManager::getInstance().isKeyPressedThisFrame(KeyCode::Space) && !_surfBoardBody->isJumping())
    {
        static_cast<physx::PxRigidBody*>(_surferBody->getBody())->addForce(
            physx::PxVec3(0.0f, 60000.0f, 0.0f),
            physx::PxForceMode::eIMPULSE
        );
    }

    float torqueAmount = 100000.0f;
    float forceAmount = 500000.0f;

	if(InputManager::getInstance().isKeyPressed(KeyCode::A))
    {
        static_cast<physx::PxRigidBody*>(_surferBody->getBody())->addForce(
            physx::PxVec3(0.5f, 0.0f, 0.0f),
            physx::PxForceMode::eVELOCITY_CHANGE
        );
        static_cast<physx::PxRigidBody*>(_surfBoardBody->getBody())->addForce(
            physx::PxVec3(0.5f, 0.0f, 0.0f),
            physx::PxForceMode::eVELOCITY_CHANGE
        );
    }
	if(InputManager::getInstance().isKeyPressed(KeyCode::D))
    {
        static_cast<physx::PxRigidBody*>(_surferBody->getBody())->addForce(
            physx::PxVec3(-0.5f, 0.0f, 0.0f),
            physx::PxForceMode::eVELOCITY_CHANGE
        );
        static_cast<physx::PxRigidBody*>(_surfBoardBody->getBody())->addForce(
            physx::PxVec3(-0.5f, 0.0f, 0.0f),
            physx::PxForceMode::eVELOCITY_CHANGE
        );
    }

	if(InputManager::getInstance().isKeyPressed(KeyCode::ArrowUp))
    {
        static_cast<physx::PxRigidBody*>(_surferBody->getBody())->addTorque(
            torqueAmount * _surferBody->getDirectionRight(),
            physx::PxForceMode::eIMPULSE
        );
    }
	if(InputManager::getInstance().isKeyPressed(KeyCode::ArrowDown))
    {
        static_cast<physx::PxRigidBody*>(_surferBody->getBody())->addTorque(
            -torqueAmount * _surferBody->getDirectionRight(),
            physx::PxForceMode::eIMPULSE
        );
    }
	if(InputManager::getInstance().isKeyPressed(KeyCode::ArrowLeft))
    {
        static_cast<physx::PxRigidBody*>(_surferBody->getBody())->addTorque(
            torqueAmount * _surferBody->getDirectionUp(),
            physx::PxForceMode::eIMPULSE
        );
    }
	if(InputManager::getInstance().isKeyPressed(KeyCode::ArrowRight))
    {
        static_cast<physx::PxRigidBody*>(_surferBody->getBody())->addTorque(
            -torqueAmount * _surferBody->getDirectionUp(),
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

    float diff = _traversalVelocity.z - _lastTraversalVelocity.z;
    if (diff > 0.01f || diff < -0.01f) {
        int newRateSurfer = _splashBaseEmissionRate_Surfer * _traversalVelocity.z / 5.0f;
        _splashSystem->setEmissionRate(newRateSurfer);

        int newRateBoat = _splashBaseEmissionRate_Boat * _traversalVelocity.z / 5.0f;
        _splashSystemBoat->setEmissionRate(newRateBoat);
    }

    _lastTraversalVelocity = _traversalVelocity;
}

void WakeboardScene::updateRain()
{
//     physx::PxVec3 boatPos = static_cast<physx::PxRigidBody*>(_boat->getBody())->getGlobalPose().p;
//     _rainSystem->setEmitterOrigin(
//         physx::PxVec3(boatPos.x, _rainSystem->getEmitterOrigin().y, boatPos.z + 10.0f)
//     );
}

void WakeboardScene::setCamera()
{
    Camera& camera = dynamic_cast<GameApp&>(GameApp::getInstance()).getCamera();


    physx::PxVec3 eye = static_cast<physx::PxRigidBody*>(_surferBody->getBody())->getGlobalPose().p + physx::PxVec3(0.0f, 5.0f, -15.0f);

    if (_surfBoardBody->isJumping()) {
        eye.y = _lastEye.y;
        _lastEye = eye;
    }

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
