#include "RegionalForce.h"

#include "ParticleWithMass.h"
#include "Region.h"


RegionalForce::RegionalForce(const Region &region)
    : ForceField(nullptr)
    , _region(region)
{
}

RegionalForce::RegionalForce(const ParticleSystem *particleSystem, const Region &region)
    : ForceField(particleSystem)
    , _region(region)
{
}

void RegionalForce::updateForce(double deltaTime)
{
    if (_force.isZero()) {
        std::cout << "RegionalForce ID " << _id << " has zero force!" << std::endl;
    }
}

void RegionalForce::applyForceOnParticle(ParticleWithMass &particle)
{
    if (_region.type == BOX) {
        if (isParticleInsideRegion(particle)) {
            particle.applyForce(_force);
        }
    }
    else {
        std::cout << "RegionalForce ID " << _id << " has unsupported region type!" << std::endl;
    }
}

bool RegionalForce::isParticleInsideRegion(const ParticleWithMass &particle)
{
    const physx::PxVec3 &pos = particle.getPosition();
    if (_region.shape.box.contains(pos)) {
        return true;
    }
    return false;
}
