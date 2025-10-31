#pragma once

#include <PxPhysicsAPI.h>

struct Vector3Stats {
    physx::PxVec3 mean;
    physx::PxVec3 deviation;

    Vector3Stats(): mean(physx::PxVec3(0, 0, 0)), deviation(physx::PxVec3(0, 0, 0)) {}
    Vector3Stats(physx::PxVec3 mean, physx::PxVec3 deviation) : mean(mean), deviation(deviation) {}
};

struct ScalarStats {
    double mean;
    double deviation;

    ScalarStats() : mean(0.0), deviation(0.0) {}
    ScalarStats(double mean, double deviation) : mean(mean), deviation(deviation) {}
};