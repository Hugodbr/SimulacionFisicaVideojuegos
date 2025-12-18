#pragma once

#include "Scene.h"

class SurferBody;
class SurfBoardBody;
class SplashSystem;
class BoxBody;

class WakeboardScene : public Scene
{
public:
	WakeboardScene() = default;
	~WakeboardScene();
    
	void init() override;
    void update() override;

protected:
    void setCamera();
    void updateCamera();
    void freeCameraControl();

    void updateTraversal();
    void updateSurfer();
    void updateSplash();

private:
	SurferBody* _surferBody = nullptr;
	SurfBoardBody* _surfBoardBody = nullptr;
	BoxBody* _boat = nullptr;
	SplashSystem* _splashSystem = nullptr;
	SplashSystem* _splashSystemBoat = nullptr;

    // physx::PxVec3 _traversalForcePerMass = physx::PxVec3(0.0f, 0.0f, 100.0f);
    float _surferBoardMass;
    float _boatMass;

    physx::PxVec3 _traversalVelocity;
    physx::PxVec3 _lastTraversalVelocity;

    bool _freeCamera = false;
    physx::PxVec3 _lastEye;

    int _splashBaseEmissionRate_Boat = 90;
    int _splashBaseEmissionRate_Surfer = 50;

    physx::PxD6Joint* surfer_board_joint;
};