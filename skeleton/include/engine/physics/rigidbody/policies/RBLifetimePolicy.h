#pragma once

#include "Policies.h"

struct RBLifetimePolicy : public ObjectLifetimePolicy
{
    // Default constructor: everything disabled (full custom)
    RBLifetimePolicy()
        : ObjectLifetimePolicy()
    {
    }
    // Lifetime only
    RBLifetimePolicy(const ScalarStats& lifetime)
        : ObjectLifetimePolicy(lifetime)
    {
    }
    // Volume bounds
    RBLifetimePolicy(const Region& region, BoundType boundType = BoundType::SOLID)
        : ObjectLifetimePolicy(region, boundType)
    {
    }
    // Callback only
    RBLifetimePolicy(std::function<bool()> callback)
        : ObjectLifetimePolicy(callback)
    {
    }
    // Copy constructor
    RBLifetimePolicy(const RBLifetimePolicy& other)
        : ObjectLifetimePolicy(other)
    {
    }
};