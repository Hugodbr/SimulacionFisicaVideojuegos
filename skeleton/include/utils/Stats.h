#pragma once

#include <PxPhysicsAPI.h>


struct Vector3Stats {
    physx::PxVec3 mean;
    physx::PxVec3 deviation;

    Vector3Stats(): mean(physx::PxVec3(0, 0, 0)), deviation(physx::PxVec3(0, 0, 0)) {}
    Vector3Stats(physx::PxVec3 mean, physx::PxVec3 deviation) : mean(mean), deviation(deviation) {}

    // Copy constructor
    Vector3Stats(const Vector3Stats& other)
        : mean(other.mean), deviation(other.deviation)
    {
    }

    Vector3Stats& operator=(const Vector3Stats& other) {
        if (this != &other) {
            mean = other.mean;
            deviation = other.deviation;
        }
        return *this;
    }
};

struct ScalarStats {
    double mean;
    double deviation;

    ScalarStats() : mean(0.0), deviation(0.0) {}
    ScalarStats(double mean, double deviation) : mean(mean), deviation(deviation) {}

    // Copy constructor
    ScalarStats(const ScalarStats& other)
        : mean(other.mean), deviation(other.deviation)
    {
    }

    ScalarStats& operator=(const ScalarStats& other) {
        if (this != &other) {
            mean = other.mean;
            deviation = other.deviation;
        }
        return *this;
    }
};

struct ColorStats {
    physx::PxVec4 mean;
    physx::PxVec4 deviation;

    ColorStats() : mean(physx::PxVec4(1.0f, 1.0f, 1.0f, 1.0f)), deviation(physx::PxVec4(0.0f, 0.0f, 0.0f, 0.0f)) {}
    ColorStats(physx::PxVec4 mean, physx::PxVec4 deviation) : mean(mean), deviation(deviation) {}

    // Copy constructor
    ColorStats(const ColorStats& other)
        : mean(other.mean), deviation(other.deviation)
    {
    }

    ColorStats& operator=(const ColorStats& other) {
        if (this != &other) {
            mean = other.mean;
            deviation = other.deviation;
        }
        return *this;
    }
};