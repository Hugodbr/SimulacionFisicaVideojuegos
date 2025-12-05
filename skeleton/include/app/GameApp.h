#pragma once

#include "ApplicationContext.h"
#include <PxPhysicsAPI.h>

class PhysicsEngine;
class InputManager;
class SceneManager;
class Viewport;
class Camera;

class GameApp : public ApplicationContext
{
private:
    static constexpr double TIME_STEP = 1.0 / 60.0; // Fixed time step for simulation 60FPS

    PhysicsEngine& _physicsEngine;
    InputManager&  _inputManager;
    SceneManager&  _sceneManager;

public:
    static GameApp& getGameInstance() {
        return static_cast<GameApp&>(ApplicationContext::getInstance());
    }
    GameApp();
    ~GameApp();

    void runInteractive();
    void runOffline(const physx::PxU32 frameCount);

    Camera& getCamera() { return *mCamera; }

private:
    void onInit() override;
    void onUpdate(double deltaTime) override;
    void onRender() override;
    void onShutdown() override;
    void onResize(int newWidth, int newHeight) override;

    void initPhysicsEngine();
    void initSceneManager();

    void createViewportsAndCameras();

private:
    // Viewpors and cameras would be defined here
    std::vector<Viewport*> mViewPorts;
    Viewport* mViewPort;

    std::vector<Camera*> mCameras;
    Camera* mCamera;
};