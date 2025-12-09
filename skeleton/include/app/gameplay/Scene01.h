#pragma once

#include "Scene.h"

class Scene01 : public Scene
{
public:
	Scene01() = default;
	~Scene01();

	void init() override;
    void update() override;

private:
};