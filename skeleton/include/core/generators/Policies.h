#pragma once

#include <random>
#include <functional>
#include <assert.h>

#include "Region.h"

class PhysicalObject;

//========================================================================================
// OBJECT GENERATION POLICY
//========================================================================================

enum class SpawnMode { Count, Interval }; // for distinguishing constructors

struct ObjectGenerationPolicy
{
    bool useSpawnCount;     // set true to spawn according to prob distribution
    ScalarStats spawnCount;

    bool useSpawnInterval;    // for effects like thunderm which has an interval between generation (not a constant generation each frame like rain)
    ScalarStats spawnInterval;     // 

    Vector3Stats position;    // mean = center, deviation = extents or radius
    Vector3Stats velocity;    // mean = center, deviation = extents or radius
    ColorStats color;

    Region region;

    std::function<bool()> customCallback;

protected:
    double currentSpawnInterval = INT_MAX;
    double accumulator = 0.0; // auxiliar to interval evaluation

    void updatePositionFromRegion();

public:
    // CONSTRUCTORS --------------------------------------------------------------------------
    ObjectGenerationPolicy(); 
    // Spawn count and spawn interval
    ObjectGenerationPolicy(
        bool useSpawnCount,    ScalarStats spawnCount,
        bool useSpawnInterval, ScalarStats spawnInterval);
    // Only spawn count or only interval
    ObjectGenerationPolicy(SpawnMode mode, ScalarStats spawnStats);
    // For constant spawn count
    ObjectGenerationPolicy(int constantSpawnCount);
    // Copy constructor
    ObjectGenerationPolicy(const ObjectGenerationPolicy& other);

    // METHODS -----------------------------------------------------------------------------------------
    void setSpawnCount(const ScalarStats& newSpawnCount);
    void setSpawnInterval(const ScalarStats& newSpawnInterval);
    void setEmitterOrigin(const physx::PxVec3& origin);
    void setVelocity(const Vector3Stats& velocity);
    void setRegion(const Region& r);
    void setColor(const ColorStats& newColor);
    void setCustomCallback(std::function<bool()> newCustomCallback);

    physx::PxVec3 generatePosition(const std::function<double()>& distributionFunc); // generate a random spawn point
    physx::PxVec3 generateVelocity(const std::function<double()>& distributionFunc); // generate a random spawn point
    physx::PxVec4 generateColor(const std::function<double()>& distributionFunc); // generate a random color
    
    bool shouldSpawn(double distr, double deltaTime); // If should spawn at a frame when using spawn interval
    int spawnNumber(double distr) const; // How many to be spawned if spawn is not constant
};


//=========================================================================================================
// OBJECT LIFETIME POLICY
//=========================================================================================================

enum class BoundType { 
    NONE, 
    SOLID, // particle can't live outside and is always alive inside
    FADE   // there`s a region around bounds where a particle has a probability to disappear
}; 

// Lifetime policy struct to ditacte how a ObjectGenerator manages the deletion of its objects 
struct ObjectLifetimePolicy
{
    bool useLifetime;        // set to expire after time
    ScalarStats lifetime;    // statistics to determine lifetime (OBS: the particle has the information of its lifetime accumulation)

    bool useVolumeBounds;    // expire when out of region
    BoundType boundType   = BoundType::NONE;    // solid or fade

    Region region;

    ScalarStats fade;

    bool useCustomCondition; // user-defined callback
    std::function<bool()> customCallback;

    // CONSTRUCTORS ------------------------------------------------------------------------------------
    // Default constructor: everything disabled (full custom)
    ObjectLifetimePolicy();
    // Lifetime only
    ObjectLifetimePolicy(const ScalarStats& lifetime);
    // Volume bounds
    ObjectLifetimePolicy(const Region& region, BoundType boundType = BoundType::SOLID);
    // Callback only
    ObjectLifetimePolicy(std::function<bool()> callback);
    // Copy constructor
    ObjectLifetimePolicy(const ObjectLifetimePolicy& other);

    // METHODS -----------------------------------------------------------------------------------------
    void setLifetime(const ScalarStats& newLifetime);
    void setVolumeRegion(const Region& r);
    void setVolumeBoundsFadeSize(const ScalarStats& newFade);
    void setCustomCallback(std::function<bool()> newCustomCallback);

    void unsetLifetime();
    void unsetVolumeBounds();
    void unsetCustomCondition();

    bool shouldDelete(double distr, const PhysicalObject& p) const;

protected:
    bool hasLeftBounds(double distr, const PhysicalObject& p) const;
    bool hasExpired(double distr, const PhysicalObject& p) const;
    bool hasCustom(double distr, const PhysicalObject& p) const;
};

