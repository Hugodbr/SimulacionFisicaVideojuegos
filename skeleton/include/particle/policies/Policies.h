#pragma once

#include <random>
#include <functional>
#include <assert.h>

#include "Region.h"

#include "Particle.h"

//========================================================================================
// PARTICLE GENERATION POLICY
//========================================================================================

enum class SpawnMode { Count, Interval }; // for distinguishing constructors

struct ParticleGenerationPolicy
{
    bool useSpawnCount;     // set true to spawn according to prob distribution
    ScalarStats spawnCount;

    bool useSpawnInterval;    // for effects like thunderm which has an interval between generation (not a constant generation each frame like rain)
    ScalarStats spawnInterval;     // 

    Vector3Stats position;    // mean = center, deviation = extents or radius
    ColorStats color;

    Region region;

    std::function<bool(double, const Particle&)> customCallback; // custom callback to given particle

private:
    double currentSpawnInterval = INT_MAX;
    double accumulator = 0.0; // auxiliar to interval evaluation

public:

    // CONSTRUCTORS --------------------------------------------------------------------------
    // No default constructor
    ParticleGenerationPolicy(); 
    // Spawn count and spawn interval
    ParticleGenerationPolicy(
        bool useSpawnCount,    ScalarStats spawnCount,
        bool useSpawnInterval, ScalarStats spawnInterval);
    // Only spawn count or only interval
    ParticleGenerationPolicy(SpawnMode mode, ScalarStats spawnStats);
    // For constant spawn count
    ParticleGenerationPolicy(int constantSpawnCount);
    // Copy constructor
    ParticleGenerationPolicy(const ParticleGenerationPolicy& other);

    // METHODS -----------------------------------------------------------------------------------------
    void setSpawnCount(const ScalarStats& newSpawnCount);
    void setSpawnInterval(const ScalarStats& newSpawnInterval);
    void setRegion(const Region& r);
    void setColor(const ColorStats& newColor);

    physx::PxVec3 generatePosition(const std::function<double()>& distributionFunc); // generate a random spawn point
    physx::PxVec4 generateColor(const std::function<double()>& distributionFunc); // generate a random color
    
    bool shouldSpawn(double distr, double deltaTime); // If should spawn at a frame when using spawn interval
    int spawnNumber(double distr) const; // How many to be spawned if spawn is not constant
};


//=========================================================================================================
// PARTICLE LIFETIME POLICY
//=========================================================================================================

enum class BoundType { 
    NONE, 
    SOLID, // particle can't live outside and is always alive inside
    FADE   // there`s a region around bounds where a particle has a probability to disappear
}; 

// Lifetime policy struct to ditacte how a ParticleGenerator manages the deletion of its particles 
struct ParticleLifetimePolicy
{
    bool useLifetime;        // set to expire after time
    ScalarStats lifetime;    // statistics to determine lifetime (OBS: the particle has the information of its lifetime accumulation)

    bool useVolumeBounds;    // expire when out of region
    BoundType boundType   = BoundType::NONE;    // solid or fade

    Region region;

    ScalarStats fade;

    bool useCustomCondition; // user-defined callback
    std::function<bool(double, const Particle&)> customCallback; // custom callback to given particle

    // CONSTRUCTORS ------------------------------------------------------------------------------------
    // Default constructor: everything disabled (full custom)
    ParticleLifetimePolicy();
    // Lifetime only
    ParticleLifetimePolicy(const ScalarStats& lifetime);
    ParticleLifetimePolicy(const Region& region, BoundType boundType = BoundType::SOLID);
    // // Box only (solid by default)
    // ParticleLifetimePolicy(const physx::PxBounds3& box, BoundType boundType = BoundType::SOLID);
    // // Sphere only (solid by default)
    // ParticleLifetimePolicy(const Vector3Stats& sphere, BoundType boundType = BoundType::SOLID);
    // Callback only
    ParticleLifetimePolicy(std::function<bool(double, const Particle&)> callback);
    // // Combined constructor (lifetime + bounds)
    // ParticleLifetimePolicy(
    //     const ScalarStats& lifetime,
    //     const physx::PxBounds3& box,
    //     BoundType boundType = BoundType::SOLID);
    // // Combined constructor (lifetime + sphere + callback)
    // ParticleLifetimePolicy(
    //     const ScalarStats& lifetime,
    //     const Vector3Stats& sphere,
    //     BoundType boundType,
    //     std::function<bool(double, const Particle&)> callback);
    // Copy constructor
    ParticleLifetimePolicy(const ParticleLifetimePolicy& other);

    // METHODS -----------------------------------------------------------------------------------------
    void setLifetime(const ScalarStats& newLifetime);
    // void setVolumeBoundsBox(const physx::PxBounds3& newBox);
    // void setVolumeBoundsSphere(const Vector3Stats& newSphere);
    void setVolumeRegion(const Region& r);
    void setVolumeBoundsFadeSize(const ScalarStats& newFade);
    void setCustomCondition(std::function<bool(double, const Particle&)>& newCustomCallback);
    // void setVolumeMeshRegion(const MeshData& mesh); // to be implemented

    void unsetLifetime();
    void unsetVolumeBounds();
    void unsetCustomCondition();

    bool shouldDelete(double distr, const Particle& p) const;

private:
    bool hasLeftBounds(double distr, const Particle& p) const;
    bool hasExpired(double distr, const Particle& p) const;
    bool hasCustom(double distr, const Particle& p) const;
};

