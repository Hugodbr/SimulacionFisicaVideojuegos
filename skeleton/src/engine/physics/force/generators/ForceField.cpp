#include "ForceField.h"

#include "ParticleWithMass.h"

ForceField::ForceField()
    : ForceGenerator()
{ }

ForceField::ForceField(const ParticleSystem *particleSystem)
    : ForceGenerator(particleSystem)
{
    initParticleSysForceGen(const_cast<ParticleSystem*>(particleSystem));
}

void ForceField::updateField(double deltaTime)
{
    // Default implementation does nothing.
}

void ForceField::updateForce(double deltaTime)
{
    updateField(deltaTime);
    ForceGenerator::updateForce(deltaTime);
}

physx::PxVec3 ForceField::computeForceOnParticle(ParticleWithMass &particle)
{
    return _force;
}

physx::PxVec3 ForceField::computeForceOnRigidBody(RigidBody &rigidBody)
{
    return _force;
}