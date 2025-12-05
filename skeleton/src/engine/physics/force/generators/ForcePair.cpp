#include "ForcePair.h"

#include <assert.h>

SpringForce::SpringForce(ParticleWithMass *mainParticle, double k, double restingLength)
    : ForceGenerator()
    , _k(k)
    , _restingLength(restingLength)
    , _mainParticle()
{
}

SpringForce::~SpringForce()
{
}

void SpringForce::updateForce(double deltaTime)
{
    assert(_otherParticle != nullptr && "No other particle assigned!");

    // physx::PxVec3 length = relativePosVector

    // physx::PxVec3 length = relativePosVector
}

void SpringForce::setOtherParticle(ParticleWithMass *otherParticle)
{
    _otherParticle = otherParticle;
}
