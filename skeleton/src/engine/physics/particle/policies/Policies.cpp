#include "Policies.h"

#include <algorithm>



//=========================================================================================================
// PARTICLE GENERATION POLICY
//=========================================================================================================
ParticleGenerationPolicy::ParticleGenerationPolicy()
    : useSpawnCount(false),
      spawnCount(ScalarStats(0.0, 0.0)),
      useSpawnInterval(false),
      spawnInterval(ScalarStats(0.0, 0.0)),
      region(Region(POINT_3D, Vector3Stats(physx::PxVec3(0, 0, 0), physx::PxVec3(0, 0, 0)))),
      position(Vector3Stats(physx::PxVec3(0, 0, 0), physx::PxVec3(0, 0, 0))),
      currentSpawnInterval(INT_MAX),
      accumulator(0.0)
{ }

ParticleGenerationPolicy::ParticleGenerationPolicy(
    bool useSpawnCount,    ScalarStats spawnCount,
    bool useSpawnInterval, ScalarStats spawnInterval)
    : useSpawnCount(useSpawnCount)
    , spawnCount(spawnCount)
    , useSpawnInterval(useSpawnInterval)
    , spawnInterval(spawnInterval)
    , region(Region(POINT_3D, Vector3Stats(physx::PxVec3(0, 0, 0), physx::PxVec3(0, 0, 0))))
{ }

ParticleGenerationPolicy::ParticleGenerationPolicy(SpawnMode mode, ScalarStats spawnStats)
    : useSpawnCount(mode == SpawnMode::Count)
    , spawnCount(mode == SpawnMode::Count ? spawnStats : ScalarStats(0, 0))
    , useSpawnInterval(mode == SpawnMode::Interval)
    , spawnInterval(mode == SpawnMode::Interval ? spawnStats : ScalarStats(0, 0))
    , region(Region(POINT_3D, Vector3Stats(physx::PxVec3(0, 0, 0), physx::PxVec3(0, 0, 0))))
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
    , region(other.region)
    , position(other.position)
    , currentSpawnInterval(other.currentSpawnInterval)
    , accumulator(other.accumulator)
    , color(other.color)
{ }

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

void ParticleGenerationPolicy::setEmitterOrigin(const physx::PxVec3 &origin)
{
    region.moveRegionTo(origin);
    updatePositionFromRegion();
}

void ParticleGenerationPolicy::setVelocity(const Vector3Stats &velocity)
{
    this->velocity = velocity;
}

void ParticleGenerationPolicy::setRegion(const Region& r)
{
    this->region = Region(r);

    updatePositionFromRegion();
}

void ParticleGenerationPolicy::updatePositionFromRegion()
{
    switch (region.type) 
    {
    case POINT_3D: {
        position.mean = this->region.shape.point.mean;
        position.deviation = this->region.shape.point.deviation;
        break;
    }
    case BOX: {
        position.mean = this->region.shape.box.getCenter();
        position.deviation = this->region.shape.box.getDimensions()/2.0;
        break;
    }
    case SPHERE: {
        position.mean = this->region.shape.sphere.mean;
        position.deviation = this->region.shape.sphere.deviation;
        break;
    }
    case DISC: {
        position.mean = this->region.shape.disc.mean;
        position.deviation = this->region.shape.disc.deviation;
        break;
    }
    case MESH: {
        position.mean = physx::PxVec3(0, 0, 0);
        position.deviation = physx::PxVec3(0, 0, 0);
        break;
    }
    }
}

void ParticleGenerationPolicy::setColor(const ColorStats &newColor)
{
    color = newColor;
}

void ParticleGenerationPolicy::setCustomCallback(std::function<bool()> newCustomCallback) {
    customCallback = std::move(newCustomCallback);
}

// Redundancy for further implementation if needed
physx::PxVec3 ParticleGenerationPolicy::generatePosition(const std::function<double()>& distributionFunc)
{
    if (region.type == POINT_3D) {
       return position.mean;
    }
    // else if (region.type == BOX) {
    //    physx::PxVec3 generatedPosition;
    //    generatedPosition.x = position.mean.x + position.deviation.x * distributionFunc();
    //    generatedPosition.y = position.mean.y + position.deviation.y * distributionFunc();
    //    generatedPosition.z = position.mean.z + position.deviation.z * distributionFunc();
    //    std::cout << "ParticleGenerationPolicy -> generatePosition: (" << generatedPosition.x << "," << generatedPosition.y << "," << generatedPosition.z << ")" << std::endl;
    //    return generatedPosition;
    // }
    //else if (regionType == SpawnRegionType::SPHERE) {
    //    return position.mean + position.deviation * distr;
    //}
    //else if (regionType == SpawnRegionType::DISC) {
    //    return position.mean + position.deviation * distr;
    //}
    if (region.type == MESH) {
        // std::cout << "ParticleGenerationPolicy -> generatePosition: MESH region." << std::endl;
        return region.shape.mesh.randomPointOnMesh(distributionFunc);
    }

    physx::PxVec3 generatedPosition;
    generatedPosition.x = position.mean.x + position.deviation.x * distributionFunc();
    generatedPosition.y = position.mean.y + position.deviation.y * distributionFunc();
    generatedPosition.z = position.mean.z + position.deviation.z * distributionFunc();
    // std::cout << "ParticleGenerationPolicy -> generatePosition: (" << generatedPosition.x << "," << generatedPosition.y << "," << generatedPosition.z << ")" << std::endl;
    return generatedPosition;


    //return position.mean + position.deviation * distr;
}

physx::PxVec3 ParticleGenerationPolicy::generateVelocity(const std::function<double()> &distributionFunc)
{
    physx::PxVec3 generatedVelocity;
    generatedVelocity.x = velocity.mean.x + velocity.deviation.x * static_cast<float>(distributionFunc());
    generatedVelocity.y = velocity.mean.y + velocity.deviation.y * static_cast<float>(distributionFunc());
    generatedVelocity.z = velocity.mean.z + velocity.deviation.z * static_cast<float>(distributionFunc());
    return generatedVelocity;
}

physx::PxVec4 ParticleGenerationPolicy::generateColor(const std::function<double()> &distributionFunc)
{
    physx::PxVec4 generatedColor;
    generatedColor.x = color.mean.x + color.deviation.x * static_cast<float>(distributionFunc());
    generatedColor.y = color.mean.y + color.deviation.y * static_cast<float>(distributionFunc());
    generatedColor.z = color.mean.z + color.deviation.z * static_cast<float>(distributionFunc());
    generatedColor.w = color.mean.w + color.deviation.w * static_cast<float>(distributionFunc());
    return generatedColor;
}

bool ParticleGenerationPolicy::shouldSpawn(double distr, double deltaTime)
{
    //std::cout << "ParticleGenerationPolicy -> shouldSpawn." << std::endl;
    if (customCallback) {
        return customCallback();
    }
    if (useSpawnInterval) {
        //std::cout << "ParticleGenerationPolicy -> useSpawnInterval -> accumulator: " << accumulator << std::endl;
        currentSpawnInterval = spawnInterval.mean + spawnInterval.deviation * distr;
        accumulator += deltaTime;
        if (accumulator < currentSpawnInterval) {
            return false;
        }
        accumulator = 0.0;
        return true;
    }

    return true;
}

int ParticleGenerationPolicy::spawnNumber(double distr) const
{
    //std::cout << "ParticleGenerationPolicy -> spawnNumber -> distr: " << distr << std::endl;

    if (useSpawnCount) {
        return static_cast<int>(spawnCount.mean + spawnCount.deviation * distr);
    }
    else {
        return static_cast<int>(spawnCount.mean);
    }
}

//=========================================================================================================
// PARTICLE LIFETIME POLICY
//=========================================================================================================

ParticleLifetimePolicy::ParticleLifetimePolicy()
    : useLifetime(false)
    , lifetime(ScalarStats())
    , useVolumeBounds(false)
    , boundType(BoundType::NONE)
    , useCustomCondition(false)
    , customCallback(nullptr)
    , region(Region(physx::PxBounds3( physx::PxVec3(-50, -50, -50), physx::PxVec3(50, 50, 50))))
{
}

ParticleLifetimePolicy::ParticleLifetimePolicy(const ScalarStats& lifetime)
    : ParticleLifetimePolicy()
{
    setLifetime(lifetime);
}

ParticleLifetimePolicy::ParticleLifetimePolicy(const Region &region, BoundType boundType)
    : ParticleLifetimePolicy()
{
    useVolumeBounds = true;
    setVolumeRegion(region);
    this->boundType = boundType;
}

// ParticleLifetimePolicy::ParticleLifetimePolicy(const physx::PxBounds3& box, BoundType boundType)
//     : ParticleLifetimePolicy()
// {
//     useVolumeBounds = true;
//     region.shape.box = box;
//     this->boundType = boundType;
// }

// ParticleLifetimePolicy::ParticleLifetimePolicy(const Vector3Stats& sphere, BoundType boundType)
//     : ParticleLifetimePolicy()
// {
//     useVolumeBounds = true;
//     region.shape.sphere = sphere;
//     this->boundType = boundType;
// }

ParticleLifetimePolicy::ParticleLifetimePolicy(std::function<bool(double, const Particle&)> callback)
    : ParticleLifetimePolicy()
{
    useCustomCondition = true;
    customCallback = callback;
}

// ParticleLifetimePolicy::ParticleLifetimePolicy(
//     const ScalarStats& lifetime,
//     const physx::PxBounds3& box,
//     BoundType boundType)
//     : ParticleLifetimePolicy(box, boundType)
// {
//     region.shape.box = box;
//     useLifetime = true;
//     this->lifetime = lifetime;
// }

// ParticleLifetimePolicy::ParticleLifetimePolicy(
//     const ScalarStats& lifetime,
//     const Vector3Stats& sphere,
//     BoundType boundType,
//     std::function<bool(double, const Particle&)> callback)
//     : ParticleLifetimePolicy(sphere, boundType)
// {
//     region.shape.sphere = sphere;
//     useLifetime = true;
//     this->lifetime = lifetime;
//     useCustomCondition = true;
//     customCallback = callback;
// }

ParticleLifetimePolicy::ParticleLifetimePolicy(const ParticleLifetimePolicy& other)
    : useLifetime(other.useLifetime)
    , lifetime(other.lifetime)
    , useVolumeBounds(other.useVolumeBounds)
    , region(other.region)
    , boundType(other.boundType)
    , fade(other.fade)
    , useCustomCondition(other.useCustomCondition)
    , customCallback(other.customCallback)
{
}

void ParticleLifetimePolicy::setLifetime(const ScalarStats& newLifetime)
{
    useLifetime = true;
    lifetime = newLifetime;
}

// void ParticleLifetimePolicy::setVolumeBoundsBox(const physx::PxBounds3& newBox)
// {
//     assert(this->region.type == VolumeType::BOX && "Lifetime volume not a box!");

//     useVolumeBounds = true;
//     shape.box = newBox;
// }

// void ParticleLifetimePolicy::setVolumeBoundsSphere(const Vector3Stats& newSphere)
// {
//     assert(this->volumeType == VolumeType::SPHERE && "Lifetime volume not a sphere!");

//     useVolumeBounds = true;
//     shape.sphere = newSphere;
// }

void ParticleLifetimePolicy::setVolumeRegion(const Region &r)
{
    useVolumeBounds = true;

    // For mesh constrain bounding box calculation
    if (r.type == MESH) {
        // this->region = Region()
        physx::PxVec3 vmin;
        physx::PxVec3 vmax;
        for (int i = 0; i < r.shape.mesh.vertices.size(); ++i) {
            vmin.x = std::min(vmin.x, r.shape.mesh.vertices[i].x);
            vmin.y = std::min(vmin.y, r.shape.mesh.vertices[i].y);
            vmin.z = std::min(vmin.z, r.shape.mesh.vertices[i].z);
            vmax.x = std::max(vmax.x, r.shape.mesh.vertices[i].x);
            vmax.y = std::max(vmax.y, r.shape.mesh.vertices[i].y);
            vmax.z = std::max(vmax.z, r.shape.mesh.vertices[i].z);
        }

        physx::PxBounds3 box(vmin, vmax);
        this->region = Region(box);
        this->region.type = BOX;
    }
    else{
        this->region = Region(r);
    }
}

void ParticleLifetimePolicy::setVolumeBoundsFadeSize(const ScalarStats &newFade)
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

        if (region.type == BOX)
        {
            const auto& box = region.shape.box;
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
        else if (region.type == SPHERE)
        {
            const auto& sphere = region.shape.sphere;
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


