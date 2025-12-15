#include "GameApp.h"

#include "PhysicsEngine.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "app/gameplay/Scene01.h"
#include "app/gameplay/SceneBuoyance.h"
#include "app/gameplay/SceneSpring.h"
#include "app/gameplay/SceneSurfer.h"
#include "app/gameplay/SpringTestScene.h"


GameApp::GameApp()
    : _physicsEngine(PhysicsEngine::getInstance())
    , _inputManager(InputManager::getInstance())
    , _sceneManager(SceneManager::getInstance())
{
}

GameApp::~GameApp()
{
    for (Viewport* vp : mViewPorts) {
        delete vp;
    }
    mViewPorts.clear();

    for (Camera* cam : mCameras) {
        delete cam;
    }
    mCameras.clear();
}

void GameApp::runInteractive()
{
    ApplicationContext::init("Game", "GLFW");
    ApplicationContext::run();
}

void GameApp::runOffline(const physx::PxU32 frameCount)
{
    for (physx::PxU32 i = 0; i < frameCount; ++i) {
        _physicsEngine.stepSimulation(TIME_STEP);
    }
}
    
void GameApp::onInit()
{
    initPhysicsEngine();

    // Start the physics simulation thread at the same timestep used by the rest of the app.
    // Uses TIME_STEP defined elsewhere in the project.
    // _physicsEngine.startSimulationThread(TIME_STEP);

    initSceneManager();
    createViewportsAndCameras();
}

void GameApp::onUpdate(double deltaTime)
{
    // std::cout << "GameApp updating..." << std::endl;
    // std::cout << "Delta Time: " << deltaTime << " seconds." << std::endl;

    mCamera->update(deltaTime);
    _sceneManager.update(deltaTime);

    // ! Fixed time step for physics simulation stability
    static double accumulator = 0.0;
    accumulator += deltaTime;
    while (accumulator >= TIME_STEP) {
        _physicsEngine.stepSimulation(TIME_STEP);
        accumulator -= TIME_STEP;
    }
}

void GameApp::onRender()
{
    // std::cout << "GameApp rendering..." << std::endl;

    _sceneManager.render(mCamera);
}

void GameApp::onShutdown()
{
    // Ensure the simulation thread is stopped before tearing down physics.
    // _physicsEngine.stopSimulationThread();

    _sceneManager.shutdown();
    _physicsEngine.shutdown();
}

void GameApp::onResize(int newWidth, int newHeight)
{
    
}

//============================= Initialization helpers ============================//

void GameApp::initPhysicsEngine()
{
    _physicsEngine.init();

    // Set up collision callbacks
    _physicsEngine.setOnCollisionCallback([](physx::PxActor* actor1, physx::PxActor* actor2) {
        std::cout << "Collision detected between actors. Callback triggered." << std::endl;
        static_cast<RigidBody*>(actor1->userData)->onCollision(actor2);
        static_cast<RigidBody*>(actor2->userData)->onCollision(actor1);
    });

    // Set up trigger callbacks
    _physicsEngine.setOnTriggerEnterCallback([](physx::PxTriggerPair* pairs, physx::PxU32 count) {
        static_cast<RigidBody*>(pairs->triggerActor->userData)->onTriggerEnter(pairs->otherActor);
    });
    _physicsEngine.setOnTriggerExitCallback([](physx::PxTriggerPair* pairs, physx::PxU32 count) {
        static_cast<RigidBody*>(pairs->triggerActor->userData)->onTriggerExit(pairs->otherActor);
    });
}

void GameApp::initSceneManager()
{
    _sceneManager.init();
    // _sceneManager.pushScene(new Scene01());
    // _sceneManager.pushScene(new SceneBuoyance());
    // _sceneManager.pushScene(new SceneSpring());
    // _sceneManager.pushScene(new SceneSurfer());
    _sceneManager.pushScene(new SpringTestScene());
}

void GameApp::createViewportsAndCameras()
{
    // VIEWPORTS ----------------------------------------------------
	Viewport* mainViewPort = new Viewport(getWindowWidth(), getWindowHeight());
	mViewPort = mainViewPort;
	mViewPorts.push_back(mainViewPort);

	// CAMARAS ------------------------------------------------------
	Camera* mainCamera = new Camera(mainViewPort, physx::PxVec3(3.0f, 3.0f, 3.0f), physx::PxVec3(.0f, .0f, .0f));
	mCamera = mainCamera;
	mCameras.push_back(mainCamera);
    _windowSystem->addInputEventListener(mCamera);

    mCamera->set3D();
}
