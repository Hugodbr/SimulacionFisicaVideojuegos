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

    void updateTraversal();
    void updateSurfer();
    void updateSplash();

private:
	SurferBody* _surferBody = nullptr;
	SurfBoardBody* _surfBoardBody = nullptr;
	BoxBody* _boat = nullptr;
	SplashSystem* _splashSystem = nullptr;

    // physx::PxVec3 _traversalForcePerMass = physx::PxVec3(0.0f, 0.0f, 100.0f);
    float _surferBoardMass;
    float _boatMass;

    physx::PxVec3 _traversalVelocity;
};