#pragma once

#include "Scene.h"

class SurferBody;
class SurfBoardBody;
class SplashSystem;
class BoxBody;

class RopeTestScene : public Scene
{
public:
	RopeTestScene() = default;
	~RopeTestScene();
    
	void init() override;
    void update() override;

private:
	SurferBody* _surferBody = nullptr;
	SurfBoardBody* _surfBoardBody = nullptr;
	BoxBody* _boat = nullptr;
	SplashSystem* _splashSystem = nullptr;
};