#pragma once

#include "Scene.h"


class SceneBuoyance : public Scene
{
public:
	SceneBuoyance() = default;
	~SceneBuoyance();

	void init() override;
    void update() override;

private:
};