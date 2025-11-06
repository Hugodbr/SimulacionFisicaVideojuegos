#include "ForceGenerator.h"

#include "Constants.h"
#include "ParticleSystem.h"

// Initialize static member
uint64_t ForceGenerator::_nextId = 0;

ForceGenerator::ForceGenerator()
    : _id(_nextId++)
    , _active(true)
    , _dead(false)
    , _magnitude(0.0f)
    , _force(physx::PxVec3(0.0f, 0.0f, 0.0f))
    , _direction(physx::PxVec3(0.0f, 0.0f, 0.0f))
    , _age(0.0)
    , _expireTime(0.0)
    , _timer(0.0)
    , _particleSystem(nullptr)
{
    std::cout << "Creating ForceGenerator with no associated ParticleSystem! ID: " << _id << std::endl;
}

ForceGenerator::ForceGenerator(const ParticleSystem *particleSystem)
    : _id(_nextId++)
    , _active(true)
    , _dead(false)
    , _magnitude(0.0f)
    , _force(physx::PxVec3(0.0f, 0.0f, 0.0f))
    , _direction(physx::PxVec3(0.0f, 0.0f, 0.0f))
    , _age(0.0)
    , _expireTime(0.0)
    , _timer(0.0)
{
    initParticleSysForceGen(const_cast<ParticleSystem*>(particleSystem));
}

void ForceGenerator::setMagnitude(float magnitude)
{
    _magnitude = magnitude;
    _force = _direction * _magnitude;
}

void ForceGenerator::setForce(const physx::PxVec3& force)
{
    _force = force;
    _magnitude = force.magnitude();

    if (!_force.isZero()) {
        _direction = force.getNormalized();
    }
    else {
        _direction = physx::PxVec3(0.0f, 0.0f, 0.0f); // Zero direction
    }
}

void ForceGenerator::setForceDirection(const physx::PxVec3 &direction)
{
    _direction = direction.getNormalized();
    _force = _direction * _magnitude;
}

void ForceGenerator::updateForce(double deltaTime)
{
    updateAge(deltaTime);
}

void ForceGenerator::updateAge(double deltaTime)
{
    if (_age > _expireTime) {
        this->setDead();
        return;
    }
    if (_timer < 0.0) {
        this->setActive(true);
    }
    else {
        _timer -= deltaTime;
    }

    _age += deltaTime;
}

physx::PxVec3 ForceGenerator::getForceOnParticle(ParticleWithMass &particle)
{
    return computeForceOnParticle(particle);
}

physx::PxVec3 ForceGenerator::computeForceOnParticle(ParticleWithMass &particle)
{
    return _force;
}
