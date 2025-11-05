#pragma once

#include "Stats.h"
#include "MeshData.h"


enum RegionType {
    BOX,
    POINT_3D,
    SPHERE,
    DISC,
    MESH
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
        }
    }

    void moveRegionTo(const physx::PxVec3& position) {
        switch (type) {
        case POINT_3D:
            shape.point.mean = position;
            break;
        case BOX:
            shape.box.minimum = position - (shape.box.maximum - shape.box.minimum) / 2;
            shape.box.maximum = position + (shape.box.maximum - shape.box.minimum) / 2;
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
        }
    }
};