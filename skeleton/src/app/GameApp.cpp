#include "GameApp.h"

#include "PhysicsEngine.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "app/gameplay/Scene01.h"


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
    mCamera->update(deltaTime);
    _sceneManager.update(deltaTime);
    _physicsEngine.stepSimulation(deltaTime);
}

void GameApp::onRender()
{
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
}

void GameApp::initSceneManager()
{
    _sceneManager.init();
    _sceneManager.pushScene(new Scene01());
}

void GameApp::createViewportsAndCameras()
{
    // VIEWPORTS ----------------------------------------------------
	Viewport* mainViewPort = new Viewport(getWindowWidth(), getWindowHeight());
	mViewPort = mainViewPort;
	mViewPorts.push_back(mainViewPort);

	// CAMARAS ------------------------------------------------------
	Camera* mainCamera = new Camera(mainViewPort, physx::PxVec3(30.0f, 30.0f, 30.0f), physx::PxVec3(.0f, .0f, .0f));
	mCamera = mainCamera;
	mCameras.push_back(mainCamera);
    _windowSystem->addInputEventListener(mCamera);

    mCamera->set3D();
}
