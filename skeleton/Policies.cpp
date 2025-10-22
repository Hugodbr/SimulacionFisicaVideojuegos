#include "Policies.h"

#include "Particle.h"


//=========================================================================================================
// PARTICLE GENERATION POLICY
//=========================================================================================================

ParticleGenerationPolicy::ParticleGenerationPolicy()
    : useSpawnCount(false),
    spawnCount(ScalarStats(0.0, 0.0)),
    useSpawnInterval(false),
    spawnInterval(ScalarStats(0.0, 0.0)),
    regionType(SpawnRegionType::POINT),
    position(Vector3Stats(physx::PxVec3(0, 0, 0), physx::PxVec3(0, 0, 0))),
    currentSpawnInterval(INT_MAX),
    accumulator(0.0)
{
    // Initialize union with a default shape based on the region type
    new (&shape.point) Vector3Stats(physx::PxVec3(0, 0, 0), physx::PxVec3(0, 0, 0));
}

ParticleGenerationPolicy::ParticleGenerationPolicy(
    bool useSpawnCount,    ScalarStats spawnCount,
    bool useSpawnInterval, ScalarStats spawnInterval)
    : useSpawnCount(useSpawnCount)
    , spawnCount(spawnCount)
    , useSpawnInterval(useSpawnInterval)
    , spawnInterval(spawnInterval)
{ }

ParticleGenerationPolicy::ParticleGenerationPolicy(SpawnMode mode, ScalarStats spawnStats)
    : useSpawnCount(mode == SpawnMode::Count)
    , spawnCount(mode == SpawnMode::Count ? spawnStats : ScalarStats(0, 0))
    , useSpawnInterval(mode == SpawnMode::Interval)
    , spawnInterval(mode == SpawnMode::Interval ? spawnStats : ScalarStats(0, 0))
    , regionType(SpawnRegionType::POINT)
    , position(Vector3Stats())
    , currentSpawnInterval(INT_MAX)
    , accumulator(0.0)
{ }

ParticleGenerationPolicy::ParticleGenerationPolicy(int constantSpawnCount)
    : ParticleGenerationPolicy(false, ScalarStats(constantSpawnCount, 0.0), false, ScalarStats())
{ }

ParticleGenerationPolicy::ParticleGenerationPolicy(const ParticleGenerationPolicy& other)
    : useSpawnCount(other.useSpawnCount)
    , spawnCount(other.spawnCount)
    , useSpawnInterval(other.useSpawnInterval)
    , spawnInterval(other.spawnInterval)
    , regionType(other.regionType)
    , position(other.position)
    , currentSpawnInterval(other.currentSpawnInterval)
    , accumulator(other.accumulator)
{
    switch (regionType)
    {
    case SpawnRegionType::POINT:
        new (&shape.point) Vector3Stats(other.shape.point);
        break;
    case SpawnRegionType::BOX:
        new (&shape.box) physx::PxBounds3(other.shape.box);
        break;
    case SpawnRegionType::SPHERE:
        new (&shape.sphere) Vector3Stats(other.shape.sphere);
        break;
    case SpawnRegionType::DISC:
        new (&shape.disc) Vector3Stats(other.shape.disc);
        break;
    case SpawnRegionType::MESH:
        // Mesh volume copy, if implemented later // TODO
        break;
    }
}

void ParticleGenerationPolicy::setSpawnCount(const ScalarStats& newSpawnCount)
{
    useSpawnCount = true;
    spawnCount = newSpawnCount;
}

void ParticleGenerationPolicy::setSpawnInterval(const ScalarStats& newSpawnInterval)
{
    useSpawnInterval = true;
    spawnInterval = newSpawnInterval;
}

void ParticleGenerationPolicy::setRegion(SpawnRegionType type, const volumeShape& shape)
{
    switch (type) 
    {
    case SpawnRegionType::POINT: {
        this->shape.point = shape.point;

        position.mean = shape.point.mean;
        position.deviation = shape.point.deviation;
        break;
    }
    case SpawnRegionType::BOX: {
        this->shape.box = shape.box;

        position.mean = shape.box.getCenter();
        position.deviation = shape.box.getDimensions()/2.0;
        break;
    }
    case SpawnRegionType::SPHERE: {
        this->shape.sphere = shape.sphere;

        position.mean = shape.sphere.mean;
        position.deviation = shape.sphere.deviation;
        break;
    }
    case SpawnRegionType::DISC: {
        this->shape.disc = shape.disc;

        position.mean = shape.disc.mean;
        position.deviation = shape.disc.deviation;
        break;
    }
    }
}

// Redundancy for further implementation if needed
physx::PxVec3 ParticleGenerationPolicy::generatePosition(double distr)
{
    if (regionType == SpawnRegionType::POINT) {
        return position.mean;
    }
    else if (regionType == SpawnRegionType::BOX) {
        return position.mean + position.deviation * distr;
    }
    else if (regionType == SpawnRegionType::SPHERE) {
        return position.mean + position.deviation * distr;
    }
    else if (regionType == SpawnRegionType::DISC) {
        return position.mean + position.deviation * distr;
    }

    return position.mean + position.deviation * distr;
}

bool ParticleGenerationPolicy::shouldSpawn(double distr, double deltaTime)
{
    if (useSpawnInterval) {
        accumulator += deltaTime;
        if (accumulator < currentSpawnInterval) {
            return false;
        }
        currentSpawnInterval = spawnInterval.mean + spawnInterval.deviation * distr;
        accumulator = 0.0;
        return true;
    }

    return true;
}

int ParticleGenerationPolicy::spawnNumber(double distr)
{
    if (useSpawnCount) {
        return spawnCount.mean + spawnCount.deviation * distr;
    }
    else {
        return spawnCount.mean;
    }
}

//=========================================================================================================
// PARTICLE LIFETIME POLICY
//=========================================================================================================

ParticleLifetimePolicy::ParticleLifetimePolicy()
    : useLifetime(false)
    , lifetime(ScalarStats())
    , useVolumeBounds(false)
    , volumeType(VolumeType::NONE)
    , boundType(BoundType::NONE)
    , useCustomCondition(false)
    , customCallback(nullptr)
{
}

ParticleLifetimePolicy::ParticleLifetimePolicy(const ScalarStats& lifetime)
    : ParticleLifetimePolicy()
{
    setLifetime(lifetime);
}

ParticleLifetimePolicy::ParticleLifetimePolicy(const physx::PxBounds3& box, BoundType boundType)
    : ParticleLifetimePolicy()
{
    useVolumeBounds = true;
    volumeType = VolumeType::BOX;
    shape.box = box;
    this->boundType = boundType;
}

ParticleLifetimePolicy::ParticleLifetimePolicy(const Vector3Stats& sphere, BoundType boundType)
    : ParticleLifetimePolicy()
{
    useVolumeBounds = true;
    volumeType = VolumeType::SPHERE;
    shape.sphere = sphere;
    this->boundType = boundType;
}

ParticleLifetimePolicy::ParticleLifetimePolicy(std::function<bool(double, const Particle&)> callback)
    : ParticleLifetimePolicy()
{
    useCustomCondition = true;
    customCallback = callback;
}

ParticleLifetimePolicy::ParticleLifetimePolicy(
    const ScalarStats& lifetime,
    const physx::PxBounds3& box,
    BoundType boundType)
    : ParticleLifetimePolicy(box, boundType)
{
    useLifetime = true;
    this->lifetime = lifetime;
}

ParticleLifetimePolicy::ParticleLifetimePolicy(
    const ScalarStats& lifetime,
    const Vector3Stats& sphere,
    BoundType boundType,
    std::function<bool(double, const Particle&)> callback)
    : ParticleLifetimePolicy(sphere, boundType)
{
    useLifetime = true;
    this->lifetime = lifetime;
    useCustomCondition = true;
    customCallback = callback;
}

ParticleLifetimePolicy::ParticleLifetimePolicy(const ParticleLifetimePolicy& other)
    : useLifetime(other.useLifetime)
    , lifetime(other.lifetime)
    , useVolumeBounds(other.useVolumeBounds)
    , volumeType(other.volumeType)
    , boundType(other.boundType)
    , fade(other.fade)
    , useCustomCondition(other.useCustomCondition)
    , customCallback(other.customCallback)
{
    switch (volumeType)
    {
    case VolumeType::BOX:
        new (&shape.box) physx::PxBounds3(other.shape.box);
        break;
    case VolumeType::SPHERE:
        new (&shape.sphere) Vector3Stats(other.shape.sphere);
        break;
    case VolumeType::NONE:
        break;
    }
}

void ParticleLifetimePolicy::setLifetime(const ScalarStats& newLifetime)
{
    useLifetime = true;
    lifetime = newLifetime;
}

void ParticleLifetimePolicy::setVolumeBoundsBox(const physx::PxBounds3& newBox)
{
    assert(this->volumeType == VolumeType::BOX && "Lifetime volume not a box!");

    useVolumeBounds = true;
    shape.box = newBox;
}

void ParticleLifetimePolicy::setVolumeBoundsSphere(const Vector3Stats& newSphere)
{
    assert(this->volumeType == VolumeType::SPHERE && "Lifetime volume not a sphere!");

    useVolumeBounds = true;
    shape.sphere = newSphere;
}

void ParticleLifetimePolicy::setVolumeBoundsFadeSize(const ScalarStats& newFade)
{
    assert(this->boundType == BoundType::FADE && "Lifetime bound type not fade!");

    fade = newFade;
}

void ParticleLifetimePolicy::setCustomCondition(std::function<bool(double, const Particle&)>& newCustomCallback)
{
    useCustomCondition = true;
    customCallback = newCustomCallback;
}

void ParticleLifetimePolicy::unsetLifetime() {
    useLifetime = false;
}

void ParticleLifetimePolicy::unsetVolumeBounds() {
    useVolumeBounds = false;
}

void ParticleLifetimePolicy::unsetCustomCondition() {
    useCustomCondition = false;
}

bool ParticleLifetimePolicy::shouldDelete(double distr, const Particle& p) const
{
    if (hasLeftBounds(distr, p) || hasExpired(distr, p) || hasCustom(distr, p)) {
        return true;
    }
    return false;
}

bool ParticleLifetimePolicy::hasLeftBounds(double distr, const Particle& p) const
{
    if (useVolumeBounds) {

        const physx::PxVec3& pos = p.getPosition();

        if (volumeType == VolumeType::BOX)
        {
            const auto& box = shape.box;
            bool inside =
                pos.x >= box.minimum.x && pos.x <= box.maximum.x &&
                pos.y >= box.minimum.y && pos.y <= box.maximum.y &&
                pos.z >= box.minimum.z && pos.z <= box.maximum.z;

            if (boundType == BoundType::SOLID)
                return !inside;

            if (boundType == BoundType::FADE)
            {
                // Create a fade margin from fade.mean + deviation*distr
                double fadeMargin = fade.mean + fade.deviation * distr;
                physx::PxBounds3 innerFadeBox(
                    box.minimum + physx::PxVec3(fadeMargin),
                    box.maximum - physx::PxVec3(fadeMargin)
                );

                bool insideFade =
                    pos.x >= innerFadeBox.minimum.x && pos.x <= innerFadeBox.maximum.x &&
                    pos.y >= innerFadeBox.minimum.y && pos.y <= innerFadeBox.maximum.y &&
                    pos.z >= innerFadeBox.minimum.z && pos.z <= innerFadeBox.maximum.z;

                // If inside outer box but outside fade box => deletion
                if (inside && !insideFade) {
                    return true;
                }

                return !inside; // outside outer box => immediate death
            }
        }
        else if (volumeType == VolumeType::SPHERE)
        {
            const auto& sphere = shape.sphere;
            physx::PxVec3 diff = pos - sphere.mean;
            float dist = diff.magnitude();
            float radius = sphere.deviation.x;

            if (boundType == BoundType::SOLID)
                return dist > radius;

            if (boundType == BoundType::FADE)
            {
                double fadeMargin = fade.mean + fade.deviation * distr;
                float innerRadius = radius - fadeMargin;
                if (dist <= radius && dist > innerRadius) {
                    return true;
                }
                return dist > radius;
            }
        }

        return false; // no volume
    }

    return false;
}

bool ParticleLifetimePolicy::hasExpired(double distr, const Particle& p) const
{
    if (useLifetime) {
        double lifetimeLimit = lifetime.mean + lifetime.deviation * distr;
        if (p.getAge() > lifetimeLimit) {
            return true;
        }
    }
    return false;
}

bool ParticleLifetimePolicy::hasCustom(double distr, const Particle& p) const
{
    if (useCustomCondition) {
        return customCallback(distr, p);
    }
    return false;
}


