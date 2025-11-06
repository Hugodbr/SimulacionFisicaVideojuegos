#pragma once

#include <iostream>

#include <PxPhysicsAPI.h>

#include "Constants.h"

class ParticleSystem;
class ParticleWithMass;


class ForceGenerator
{
private:
    static uint64_t _nextId; // Static counter for generating unique IDs

protected:
    // The group this force applies to. Forces can be applied selectively based on group.
    Constants::Group::DynamicGroup _group = Constants::Group::DynamicGroup::ALL;
    uint64_t _id; // Protected to allow derived classes access

public:
    // For global forces, particleSystem can be nullptr
    ForceGenerator();
    ForceGenerator(const ParticleSystem* particleSystem);
    virtual ~ForceGenerator() = default;

    virtual void updateForce(double deltaTime);
    virtual void updateAge(double deltaTime);

    virtual physx::PxVec3 getForceOnParticle(ParticleWithMass& particle);

    uint64_t getId() const { return _id; }

    void initParticleSysForceGen(ParticleSystem* particleSystem) {
        _particleSystem = particleSystem;
    }

    virtual void setMagnitude(float magnitude);
    virtual void setForceDirection(const physx::PxVec3& direction);
    virtual void setForce(const physx::PxVec3& force);

    Constants::Group::DynamicGroup getGroup() const { return _group; }
    void setGroup(Constants::Group::DynamicGroup group) { _group = group; }

    virtual void setExpireTime(double time) { _expireTime = time; }
    virtual void setTimer(double time) { _timer = time; }

    // Activate or deactivate this force generator. Its effect will be considered only if active.
    virtual void setActive(bool active) { _active = active; }
    bool isActive() const { return _active; }
    // Mark this force generator as dead (to be removed and deleted)
    void setDead() { 
        _active = false;
        _dead = true; 
    }
    bool isDead() const { return _dead; }

    void setParticleSystem(ParticleSystem* particleSystem) { _particleSystem = particleSystem; }
    // Getter for ParticleSystem. Caution: may return nullptr if not set.
    ParticleSystem* getParticleSystem() const { return _particleSystem; }

protected:
    virtual physx::PxVec3 computeForceOnParticle(ParticleWithMass& particle);

protected:
    physx::PxVec3 _force;
    float _magnitude;
    physx::PxVec3 _direction;

    ParticleSystem* _particleSystem; // The particle system this force generator is associated with

    double _age;
    double _expireTime;
    double _timer;

    bool _active;
    bool _dead;
};