#pragma once

#include "Scene.h"


class CylinderTestScene : public Scene
{
public:
	CylinderTestScene() = default;
	~CylinderTestScene();
    
	void init() override;
    void update() override;

private:
};