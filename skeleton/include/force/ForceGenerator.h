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
    uint64_t _id; // Protected to allow derived classes access

public:
    // For global forces, particleSystem can be nullptr
    ForceGenerator();
    
    ForceGenerator(const ParticleSystem* particleSystem);

    virtual ~ForceGenerator() = default;

    virtual void updateForce(double deltaTime) = 0;
    virtual void applyForceOnParticle(ParticleWithMass& particle) = 0;

    uint64_t getId() const { return _id; }

    void initParticleSysForceGen(ParticleSystem* particleSystem) {
        _particleSystem = particleSystem;
    }

    // Activate or deactivate this force generator. Its effect will be considered only if active.
    virtual void setActive(bool active) { _active = active; }
    bool isActive() const { return _active; }
    // Mark this force generator as dead (to be removed and deleted)
    void setDead(bool dead) { _dead = dead; }
    bool isDead() const { return _dead; }

    void setParticleSystem(ParticleSystem* particleSystem) { _particleSystem = particleSystem; }
    // Getter for ParticleSystem. Caution: may return nullptr if not set.
    ParticleSystem* getParticleSystem() const { return _particleSystem; }

protected:
    physx::PxVec3 _force;
    float _magnitude;

    ParticleSystem* _particleSystem; // The particle system this force generator is associated with

    bool _active;
    bool _dead;
};