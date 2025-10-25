#include "StaticParticle.h"


StaticParticle::StaticParticle(float size, const physx::PxVec4& color)
    : Particle(
        physx::PxTransform(), // initial transform at origin
        physx::PxVec3(0, 0, 0), // initial velocity
        physx::PxVec3(0, 0, 0), // initial acceleration
        Constants::Integration_Method::NONE, // static
        size,
        1.0, // damping
        color
    )
{

}