#pragma once

#include "Scene.h"

// class Particle;
// class ParticleSystem;

class Scene01 : public Scene
{
public:
	Scene01() = default;
	~Scene01();

	void init() override;
    void update() override;

private:
	// Particle* _particle = nullptr;
	// Particle* _particle2 = nullptr;
};