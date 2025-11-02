#include "ForceField.h"

#include "ParticleWithMass.h"

ForceField::ForceField()
    : ForceGenerator()
{
}

ForceField::ForceField(const ParticleSystem *particleSystem)
    : ForceGenerator(particleSystem)
{
    initParticleSysForceGen(const_cast<ParticleSystem*>(particleSystem));
}

void ForceField::updateForce(double deltaTime)
{
    std::cout << "Updating ForceField ID: " << _id << ". Caution: pure force field!" << std::endl;
}

void ForceField::applyForceOnParticle(ParticleWithMass &particle)
{
    physx::PxVec3 force = computeForceOnParticle(particle);
    particle.applyForce(force);
}

physx::PxVec3 ForceField::computeForceOnParticle(ParticleWithMass &particle)
{
    return physx::PxVec3(0, 0, 0);
}
