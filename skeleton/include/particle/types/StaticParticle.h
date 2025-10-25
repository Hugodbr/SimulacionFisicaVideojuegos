#pragma once

#include "Particle.h"


class StaticParticle : public Particle
{
public:

    StaticParticle(
        float size = Constants::Particle::Size,
        const physx::PxVec4& color = Constants::Color::White
    );

    ~StaticParticle() = default;

};