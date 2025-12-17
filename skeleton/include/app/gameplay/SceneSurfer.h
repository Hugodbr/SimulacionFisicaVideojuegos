#pragma once

#include "Scene.h"

class SurferBody;
class SurfBoardBody;
class SplashSystem;

class SceneSurfer : public Scene
{
public:
	SceneSurfer() = default;
	~SceneSurfer();
    
	void init() override;
    void update() override;

private:
	SurferBody* _surferBody = nullptr;
	SurfBoardBody* _surfBoardBody = nullptr;
	SplashSystem* _splashSystem = nullptr;
};