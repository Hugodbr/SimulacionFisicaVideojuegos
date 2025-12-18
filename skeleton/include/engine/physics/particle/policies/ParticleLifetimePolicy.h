#pragma once

#include "Policies.h"

struct ParticleLifetimePolicy : public ObjectLifetimePolicy
{
    // Default constructor: everything disabled (full custom)
    ParticleLifetimePolicy()
        : ObjectLifetimePolicy()
    {
    }
    // Lifetime only
    ParticleLifetimePolicy(const ScalarStats& lifetime)
        : ObjectLifetimePolicy(lifetime)
    {
    }
    // Volume bounds
    ParticleLifetimePolicy(const Region& region, BoundType boundType = BoundType::SOLID)
        : ObjectLifetimePolicy(region, boundType)
    {
    }
    // Callback only
    ParticleLifetimePolicy(std::function<bool()> callback)
        : ObjectLifetimePolicy(callback)
    {
    }
    // Copy constructor
    ParticleLifetimePolicy(const ParticleLifetimePolicy& other)
        : ObjectLifetimePolicy(other)
    {
    }
};