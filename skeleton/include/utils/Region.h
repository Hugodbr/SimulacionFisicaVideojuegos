#pragma once

#include <iostream>

#include "Stats.h"
#include "MeshData.h"

struct Cylinder {
    physx::PxVec3 baseCenter;
    physx::PxVec3 topCenter;
    float radius;

    Cylinder() : baseCenter(physx::PxVec3(0.0f, 0.0f, 0.0f)), topCenter(physx::PxVec3(0.0f, 1.0f, 0.0f)), radius(1.0f) {}
    Cylinder(const physx::PxVec3& base, const physx::PxVec3& top, float r) : baseCenter(base), topCenter(top), radius(r) {}

    void update(const physx::PxVec3& top, const physx::PxVec3& base, float r) {
        baseCenter = base;
        topCenter = top;
        radius = r;
    }

    bool containsPoint(const physx::PxVec3& point) const {
        physx::PxVec3 ab = topCenter - baseCenter;
        physx::PxVec3 ap = point - baseCenter;

        float ab_squared = ab.magnitudeSquared();
        float ap_dot_ab = ab.dot(ap);
        float t = ap_dot_ab / ab_squared;
        if (t < 0.0f || t > 1.0f) {
            return false;
        }
        physx::PxVec3 q = baseCenter + ab * t;
        float d = (point - q).magnitude()   ;
        return d < radius;
    }

    // Copy constructor
    Cylinder(const Cylinder& other)
        : baseCenter(other.baseCenter), topCenter(other.topCenter), radius(other.radius)
    {
    }

    Cylinder& operator=(const Cylinder& other) {
        if (this != &other) {
            baseCenter = other.baseCenter;
            topCenter = other.topCenter;
            radius = other.radius;
        }
        return *this;
    }
};

enum RegionType {
    BOX,
    POINT_3D,
    SPHERE,
    DISC,
    MESH,
    CYLINDER
};

struct Region 
{
    RegionType type;

    union volumeShape {
        Vector3Stats point;  // mean=center, deviation.x = 0
        physx::PxBounds3 box;
        Vector3Stats sphere; // mean=center, deviation.x = radius
        Vector3Stats disc;   // mean=center, deviation.x = radius XY plane)
        MeshData mesh;
        Cylinder cylinder;
        volumeShape() {}
        ~volumeShape() {}
    } shape;

    explicit Region(RegionType type, Vector3Stats stats)
        : type(type)
    {
        switch (type) {
        case POINT_3D:
            new (&shape.point) Vector3Stats(stats);
            break;
        case SPHERE:
            new (&shape.sphere) Vector3Stats(stats);
            break;
        case DISC:
            new (&shape.disc) Vector3Stats(stats);
            break;
        }
    }

    explicit Region(physx::PxBounds3 box)
        : type(BOX)
    {
        new (&shape.box) physx::PxBounds3(box);
    }

    explicit Region(MeshData meshData)
        : type(MESH)
    {
        new (&shape.mesh) MeshData(meshData);
    }

    explicit Region(Cylinder cylinder)
        : type(CYLINDER)
    {
        new (&shape.cylinder) Cylinder(cylinder);
    }

    explicit Region(const Region& other) : type(other.type) {
        switch (type) {
        case POINT_3D:
            new (&shape.point) Vector3Stats(other.shape.point);
            break;
        case BOX:
            new (&shape.box) physx::PxBounds3(other.shape.box);
            break;
        case SPHERE:
            new (&shape.sphere) Vector3Stats(other.shape.sphere);
            break;
        case DISC:
            new (&shape.disc) Vector3Stats(other.shape.disc);
            break;
        case MESH:
            new (&shape.mesh) MeshData(other.shape.mesh);
            break;
        case CYLINDER:
            new (&shape.cylinder) Cylinder(other.shape.cylinder);
            break;
        }
    }

    explicit Region(Region&& other) noexcept : type(other.type)
    {
        switch (type)
        {
        case POINT_3D:
            new (&shape.point) Vector3Stats(std::move(other.shape.point));
            break;
        case BOX:
            new (&shape.box) physx::PxBounds3(std::move(other.shape.box));
            break;
        case SPHERE:
            new (&shape.sphere) Vector3Stats(std::move(other.shape.sphere));
            break;
        case DISC:
            new (&shape.disc) Vector3Stats(std::move(other.shape.disc));
            break;
        case MESH:
            new (&shape.mesh) MeshData(std::move(other.shape.mesh));
            break;
        case CYLINDER:
            new (&shape.cylinder) Cylinder(std::move(other.shape.cylinder));
            break;
        }
    }

    Region& operator=(const Region& other)
    {
        if (this != &other) {
            this->~Region();
            new (this) Region(other);
        }
        return *this;
    }

    ~Region() {
        switch (type) {
        case POINT_3D:
            shape.point.~Vector3Stats();
            break;
        case BOX:
            shape.box.~PxBounds3();
            break;
        case SPHERE:
            shape.sphere.~Vector3Stats();
            break;
        case DISC:
            shape.disc.~Vector3Stats();
            break;
        case MESH:
            shape.mesh.~MeshData();
            break;
        case CYLINDER:
            shape.cylinder.~Cylinder();
            break;
        }
    }

    void moveRegionTo(const physx::PxVec3& position) {
        switch (type) {
        case POINT_3D:
            shape.point.mean = position;
            break;
        case BOX: {
            // std::cout << "Region::moveRegionTo BOX moving to " << position.x << ", " << position.y << ", " << position.z << std::endl;
            physx::PxVec3 size = shape.box.maximum - shape.box.minimum;
            physx::PxVec3 halfSize = size * 0.5f;
            shape.box.minimum = position - halfSize;
            shape.box.maximum = position + halfSize;
            }
            break;
        case SPHERE:
            shape.sphere.mean = position;
            break;
        case DISC:
            shape.disc.mean = position;
            break;
        case MESH:
            shape.mesh.moveMeshTo(position);
            break;
        case CYLINDER:
            std::cout << "Region::moveRegionTo CYLINDER not implemented" << std::endl;
            break;
        }
    }

    bool containsPoint(const physx::PxVec3& point) const {
        switch (type) {
        case POINT_3D:
            std::cout << "Region::containsPoint POINT_3D not implemented" << std::endl;
            return true;
        case BOX:
            return shape.box.contains(point);
        case SPHERE: {
            physx::PxVec3 diff = point - shape.sphere.mean;
            return diff.magnitudeSquared() < (shape.sphere.deviation.x * shape.sphere.deviation.x);
        }
        case DISC: {
            std::cout << "Region::containsPoint DISC not implemented" << std::endl;
            return true;
        }
        case MESH:
            return shape.mesh.isPointInsideMesh(point);
        case CYLINDER: {
            return shape.cylinder.containsPoint(point);
        }
        default:
            return false;
        }
    }
};