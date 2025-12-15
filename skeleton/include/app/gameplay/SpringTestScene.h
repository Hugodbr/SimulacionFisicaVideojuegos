#pragma once

#include "Scene.h"

class BoxBody;

class SpringTestScene : public Scene
{
public:
	SpringTestScene() = default;
	~SpringTestScene();
    
	void init() override;
    void update() override;

private:
	physx::PxD6Joint* _b1b2Joint = nullptr;
	BoxBody* _boxBody1 = nullptr;
	BoxBody* _boxBody2 = nullptr;
};