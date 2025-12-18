#pragma once

#include "Policies.h"

struct RBGenerationPolicy : public ObjectGenerationPolicy 
{
    RBGenerationPolicy()
        : ObjectGenerationPolicy()
    {
    }
    // Spawn count and spawn interval
    RBGenerationPolicy(
        bool useSpawnCount,    ScalarStats spawnCount,
        bool useSpawnInterval, ScalarStats spawnInterval)
        : ObjectGenerationPolicy(
            useSpawnCount, spawnCount,
            useSpawnInterval, spawnInterval)
    {
    }
    // Only spawn count or only interval
    RBGenerationPolicy(SpawnMode mode, ScalarStats spawnStats)
        : ObjectGenerationPolicy(mode, spawnStats)
    {
    }
    // For constant spawn count
    RBGenerationPolicy(int constantSpawnCount)
        : ObjectGenerationPolicy(constantSpawnCount)
    {
    }
    // Copy constructor
    RBGenerationPolicy(const RBGenerationPolicy& other)
        : ObjectGenerationPolicy(other)
    {
    }
};