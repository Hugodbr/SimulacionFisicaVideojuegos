#pragma once

#include "Policies.h"

struct ParticleGenerationPolicy : public ObjectGenerationPolicy 
{
    ParticleGenerationPolicy()
        : ObjectGenerationPolicy()
    {
    }
    // Spawn count and spawn interval
    ParticleGenerationPolicy(
        bool useSpawnCount,    ScalarStats spawnCount,
        bool useSpawnInterval, ScalarStats spawnInterval)
        : ObjectGenerationPolicy(
            useSpawnCount, spawnCount,
            useSpawnInterval, spawnInterval)
    {
    }
    // Only spawn count or only interval
    ParticleGenerationPolicy(SpawnMode mode, ScalarStats spawnStats)
        : ObjectGenerationPolicy(mode, spawnStats)
    {
    }
    // For constant spawn count
    ParticleGenerationPolicy(int constantSpawnCount)
        : ObjectGenerationPolicy(constantSpawnCount)
    {
    }
    // Copy constructor
    ParticleGenerationPolicy(const ParticleGenerationPolicy& other)
        : ObjectGenerationPolicy(other)
    {
    }
};