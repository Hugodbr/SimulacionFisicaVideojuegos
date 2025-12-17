#pragma once

#include "Scene.h"

class PhysicalObject;
class RigidBodySystem;

class SpringTestScene : public Scene
{
public:
	SpringTestScene() = default;
	~SpringTestScene();
    
	void init() override;
    void update() override;

private:
	PhysicalObject* _endObject = nullptr;
	RigidBodySystem* _springSystem = nullptr;
};