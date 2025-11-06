#include "StaticParticle.h"


StaticParticle::StaticParticle(float size, const physx::PxVec4& color)
    : Particle(
        size,
        color,
        0.0, // speed
        Constants::Integration_Method::NONE // static
    )
{ }