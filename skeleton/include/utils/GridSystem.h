#pragma once

#include "ParticleSystem.h"


class GridSystem : public ParticleSystem
{
public:
    GridSystem(const physx::PxBounds3 &region, float pointSize = 1.0f, double scale = 10.0, const Vector4& color = Constants::Color::White);
    ~GridSystem() = default;

    void init() override;
    void update(double deltaTime) override;

    void toggleVisibility(bool visible = false);

protected:

    void createParticleGenerator() override;

    void createParticlesInGrid();

protected:

    physx::PxBounds3 _region;
    float _pointSize;
    double _scale;
    Vector4 _color;
};