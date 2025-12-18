#include "ObjectGenerator.h"


// Must initialize
ObjectGenerator::ObjectGenerator()
    : _mt(std::random_device{}())
    , _generationPolicy(std::make_unique<ObjectGenerationPolicy>()) // Must initialize
    , _lifetimePolicy(std::make_unique<ObjectLifetimePolicy>()) // Must initialize
{ }

void ObjectGenerator::init(
    const physx::PxVec3& emitterOrigin, 
    const Vector3Stats& velocity
)
{
    _emitterOrigin = emitterOrigin;
    _meanVelocity = velocity.mean;
    _velocityDeviation = velocity.deviation;
}

void ObjectGenerator::setEmitterOrigin(const physx::PxVec3& emitterOrigin) {
    _emitterOrigin = emitterOrigin;
    _generationPolicy->setEmitterOrigin(emitterOrigin);
}

void ObjectGenerator::setMeanVelocity(const physx::PxVec3& meanVel) {
    _meanVelocity = meanVel;
}

void ObjectGenerator::setVelocityDeviation(const physx::PxVec3& velDeviation) {
    _velocityDeviation = velDeviation;
}
void ObjectGenerator::setGenerationPolicy(const ObjectGenerationPolicy& genPolicy) {
    _generationPolicy = std::make_unique<ObjectGenerationPolicy>(genPolicy);
    _generationPolicy->setEmitterOrigin(_emitterOrigin);
    _generationPolicy->setVelocity(Vector3Stats(_meanVelocity, _velocityDeviation));
}

void ObjectGenerator::setLifetimePolicy(const ObjectLifetimePolicy& lifePolicy) {
    _lifetimePolicy = std::make_unique<ObjectLifetimePolicy>(lifePolicy);
}

int ObjectGenerator::numberOfGenerations() {
    return _generationPolicy->spawnNumber(getDistribution());
}

physx::PxVec3 ObjectGenerator::getGeneratedPosition()
{
    return _generationPolicy->generatePosition([this]() {
            return getDistribution();
            });
}

physx::PxVec3 ObjectGenerator::getGeneratedVelocity()
{
    return _generationPolicy->generateVelocity([this]() {
            return getDistribution();
            });
}

physx::PxVec4 ObjectGenerator::getGeneratedColor()
{
    return _generationPolicy->generateColor([this]() {
            return getDistribution();
            });
}
