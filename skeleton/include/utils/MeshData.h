#pragma once

#include <functional>
#include <unordered_set>
#include <tuple>
#include <cmath>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "PxPhysicsAPI.h"


struct MeshData {
    std::vector<physx::PxVec3> vertices = {};
    std::vector<physx::PxVec3> uniqueVertices = {};
    std::vector<unsigned int> indices = {};

    physx::PxVec3 center = physx::PxVec3(0.0f, 0.0f, 0.0f);

    MeshData() = default;
    MeshData(const MeshData& other) {
        vertices = other.vertices;
        indices = other.indices;
        center = other.center;
    }
    MeshData(const std::string& filename) {
        loadMeshFromFile(filename);
    }

    void loadMeshFromFile(const std::string& filename);
    void readUniqueVertices(const std::string& filename);

    void computeCenter();

    void moveMeshTo(const physx::PxVec3& position);

    physx::PxVec3 randomPointOnMesh(const std::function<double()>& distributionFunc);
    std::vector<physx::PxVec3> getMeshVertices() const;

    MeshData& operator =(const MeshData& other) {
        vertices = other.vertices;
        indices = other.indices;
        uniqueVertices = other.uniqueVertices;
        return *this;
    }

    ~MeshData() = default;
};
