#include "RegionalForce.h"

#include <cassert>

#include "ParticleWithMass.h"
#include "RigidBody.h"
#include "Region.h"


RegionalForce::RegionalForce(const Region &region)
    : ForceField()
    , _region(region)
    , _follows(false)
    , _followTargetParticle(nullptr)
{
    assert((_region.type == BOX || _region.type == MESH || _region.type == SPHERE || _region.type == CYLINDER)
        && "Only BOX, MESH, SPHERE, and CYLINDER region types are currently supported in RegionalForce.");
}

RegionalForce::RegionalForce(const ParticleSystem *particleSystem, const Region &region)
    : ForceField(particleSystem)
    , _region(region)
    , _follows(false)
    , _followTargetParticle(nullptr)
{
    assert((_region.type == BOX || _region.type == MESH || _region.type == SPHERE || _region.type == CYLINDER)
        && "Only BOX, MESH, SPHERE, and CYLINDER region types are currently supported in RegionalForce.");
}

void RegionalForce::setRegion(const Region &region)
{
    _region = Region(region);

    assert((_region.type == BOX || _region.type == MESH || _region.type == SPHERE || _region.type == CYLINDER)
        && "Only BOX, MESH, SPHERE, and CYLINDER region types are currently supported in RegionalForce.");
}

void RegionalForce::setFollowParticle(bool follow, const ParticleWithMass &particle)
{
    _follows = follow;
    if (follow) {
        _followTargetParticle = &particle;
    }
}

void RegionalForce::updateField(double deltaTime)
{
    if (_follows) {
        if (!_followTargetParticle->isActive()) {
            this->setDead();
        } else {
            // Move region to follow target particle
            _region.moveRegionTo(_followTargetParticle->getPosition());
        }
    }
}

void RegionalForce::updateForce(double deltaTime)
{
    ForceField::updateForce(deltaTime);
}

physx::PxVec3 RegionalForce::computeForceOnParticle(ParticleWithMass &particle)
{
    if (isParticleInsideRegion(particle)) {
        return _force;
    }

    return physx::PxVec3(0.0f, 0.0f, 0.0f);
}

physx::PxVec3 RegionalForce::computeForceOnRigidBody(RigidBody &rigidBody)
{
    if (isRigidBodyInsideRegion(rigidBody)) {
        return _force;
    }

    return physx::PxVec3(0.0f, 0.0f, 0.0f);
}

bool RegionalForce::isParticleInsideRegion(const ParticleWithMass &particle)
{
    const physx::PxVec3 &pos = particle.getPosition();

    if (_region.containsPoint(pos)) {
        return true;
    }
    return false;
}

bool RegionalForce::isRigidBodyInsideRegion(const RigidBody &rigidBody)
{
    const physx::PxVec3 &pos = rigidBody.getPosition();

    if (_region.containsPoint(pos)) {
        return true;
    }
    return false;
}
