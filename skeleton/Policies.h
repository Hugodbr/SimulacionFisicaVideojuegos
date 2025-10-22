#pragma once

#include <random>
#include <functional>
#include <assert.h>

#include <PxPhysicsAPI.h>

class Particle;


struct Vector3Stats {
    physx::PxVec3 mean;
    physx::PxVec3 deviation;

    Vector3Stats() : mean(physx::PxVec3(0, 0, 0)), deviation(physx::PxVec3(0, 0, 0)) {}
    Vector3Stats(physx::PxVec3 mean, physx::PxVec3 deviation) : mean(mean), deviation(deviation) {}
};

struct ScalarStats {
    double mean;
    double deviation;

    ScalarStats() : mean(0.0), deviation(0.0) {}
    ScalarStats(double mean, double deviation) : mean(mean), deviation(deviation) {}
};

//=========================================================================================================
// PARTICLE GENERATION POLICY
//=========================================================================================================

enum class SpawnMode { Count, Interval }; // for distinguishing constructors

enum class SpawnRegionType {
    POINT,
    BOX,
    SPHERE,
    DISC,
    MESH
};

struct ParticleGenerationPolicy
{
    bool useSpawnCount;     // set true to spawn according to prob distribution
    ScalarStats spawnCount;

    bool useSpawnInterval;    // for effects like thunderm which has an interval between generation (not a constant generation each frame like rain)
    ScalarStats spawnInterval;     // 

    SpawnRegionType regionType = SpawnRegionType::POINT;
    Vector3Stats position;    // mean = center, deviation = extents or radius

    union volumeShape {
        Vector3Stats point;  // mean=center, deviation.x = 0
        physx::PxBounds3 box;
        Vector3Stats sphere; // mean=center, deviation.x = radius (OBS: better with Gaussian distribution?)
        Vector3Stats disc;   // mean=center, deviation.x = radius (OBS: define plane by vector components, i.e. XY plane)
        volumeShape() {}
        ~volumeShape() {}
    } shape;

private:
    double currentSpawnInterval = INT_MAX;
    double accumulator = 0.0; // auxiliar to interval evaluation

public:

    // CONSTRUCTORS ------------------------------------------------------------------------------------
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
    void setRegion(SpawnRegionType type, const volumeShape& shape);

    physx::PxVec3 generatePosition(double distr); // generate a random spawn point
    
    bool shouldSpawn(double distr, double deltaTime); // If should spawn at a frame when using spawn interval
    int spawnNumber(double distr); // How many to be spawned if spawn is not constant
};


//=========================================================================================================
// PARTICLE LIFETIME POLICY
//=========================================================================================================

enum class VolumeType { 
    NONE, 
    BOX,   // box shape volume
    SPHERE // center with a radius
};
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
    VolumeType volumeType = VolumeType::NONE;   // shape type
    BoundType boundType   = BoundType::NONE;    // solid or fade

    union volumeShape {
        physx::PxBounds3 box;
        Vector3Stats sphere; // mean=center, deviation.x = radius (OBS: better with Gaussian distribution)
        volumeShape() {}
        ~volumeShape() {}
    } shape;

    ScalarStats fade;

    bool useCustomCondition; // user-defined callback
    std::function<bool(double, const Particle&)> customCallback; // custom callback to given particle

    // CONSTRUCTORS ------------------------------------------------------------------------------------
    // Default constructor: everything disabled (full custom)
    ParticleLifetimePolicy();
    // Lifetime only
    ParticleLifetimePolicy(const ScalarStats& lifetime);
    // Box only (solid by default)
    ParticleLifetimePolicy(const physx::PxBounds3& box, BoundType boundType = BoundType::SOLID);
    // Sphere only (solid by default)
    ParticleLifetimePolicy(const Vector3Stats& sphere, BoundType boundType = BoundType::SOLID);
    // Callback only
    ParticleLifetimePolicy(std::function<bool(double, const Particle&)> callback);
    // Combined constructor (lifetime + bounds)
    ParticleLifetimePolicy(
        const ScalarStats& lifetime,
        const physx::PxBounds3& box,
        BoundType boundType = BoundType::SOLID);
    // Combined constructor (lifetime + sphere + callback)
    ParticleLifetimePolicy(
        const ScalarStats& lifetime,
        const Vector3Stats& sphere,
        BoundType boundType,
        std::function<bool(double, const Particle&)> callback);
    // Copy constructor
    ParticleLifetimePolicy(const ParticleLifetimePolicy& other);

    // METHODS -----------------------------------------------------------------------------------------
    void setLifetime(const ScalarStats& newLifetime);
    void setVolumeBoundsBox(const physx::PxBounds3& newBox);
    void setVolumeBoundsSphere(const Vector3Stats& newSphere);
    void setVolumeBoundsFadeSize(const ScalarStats& newFade);
    void setCustomCondition(std::function<bool(double, const Particle&)>& newCustomCallback);

    void unsetLifetime();
    void unsetVolumeBounds();
    void unsetCustomCondition();

    bool shouldDelete(double distr, const Particle& p) const;

private:
    bool hasLeftBounds(double distr, const Particle& p) const;
    bool hasExpired(double distr, const Particle& p) const;
    bool hasCustom(double distr, const Particle& p) const;
};

