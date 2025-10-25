#pragma once

#include <functional>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "PxPhysicsAPI.h"


struct MeshData {
    std::vector<physx::PxVec3> vertices = {};
    std::vector<unsigned int> indices = {};

    MeshData() = default;
    MeshData(const MeshData& other) {
        vertices = other.vertices;
        indices = other.indices;
    }

    void loadMesh(const std::string& filename);

    physx::PxVec3 randomPointOnMesh(const std::function<double()>& distributionFunc);

    MeshData& operator =(const MeshData& other) {
        vertices = other.vertices;
        indices = other.indices;
        return *this;
    }
};
