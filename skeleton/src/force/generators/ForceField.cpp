#include "ForceField.h"

ForceField::ForceField()
{
}

void ForceField::updateForce(double deltaTime)
{
}

physx::PxVec3 ForceField::computeForceOnParticle(Particle &particle)
{
    return physx::PxVec3(0, 0, 0);
}
