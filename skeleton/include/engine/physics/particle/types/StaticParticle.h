#pragma once

#include "Particle.h"


class StaticParticle : public Particle
{
public:
    StaticParticle(
        float size = Constants::Particle::Default::Size,
        const physx::PxVec4& color = Constants::Color::White
    );

    virtual ~StaticParticle() = default;
};