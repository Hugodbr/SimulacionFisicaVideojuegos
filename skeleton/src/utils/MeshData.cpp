#include "MeshData.h"

#include <stdexcept>
#include <random>
#include <iostream>
#include <fstream>
#include <sstream>


void MeshData::loadMeshFromFile(const std::string& filename) 
{
    readUniqueVertices(filename);

    if (uniqueVertices.empty()) {
        throw std::runtime_error("No vertices loaded from file: " + filename);
    }
    if (uniqueVertices.size() > 3){

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(
            filename,
            aiProcess_Triangulate | aiProcess_JoinIdenticalVertices
        );

        if (!scene || !scene->HasMeshes()) {
            throw std::runtime_error("Failed to load mesh from file: " + filename);
        }

        aiMesh* m = scene->mMeshes[0];
        vertices.reserve(m->mNumVertices);
        for (unsigned int i = 0; i < m->mNumVertices; ++i) {
            vertices.emplace_back(m->mVertices[i].x, m->mVertices[i].y, m->mVertices[i].z);
        }

        for (unsigned int i = 0; i < m->mNumFaces; ++i) {
            for (unsigned int j = 0; j < m->mFaces[i].mNumIndices; ++j) {
                indices.push_back(m->mFaces[i].mIndices[j]);
            }
        }
    }

    computeCenter();
    computeAABB();
}

void MeshData::readUniqueVertices(const std::string& filename) 
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        // Skip lines that don't start with 'v ' (space is important!)
        if (line.rfind("v ", 0) == 0) {
            std::istringstream iss(line.substr(2)); // skip "v "
            float x, y, z;
            if (iss >> x >> y >> z) {
                std::cout << "Read vertex: " << x << ", " << y << ", " << z << std::endl;
                uniqueVertices.emplace_back(x, y, z);
            }
        }
    }

    file.close();
    std::cout << "Loaded " << uniqueVertices.size() << " vertices from " << filename << std::endl;
}

void MeshData::computeCenter()
{
    if (uniqueVertices.empty()) {
        center = physx::PxVec3(0.0f, 0.0f, 0.0f);
        return;
    }

    physx::PxVec3 sum(0.0f, 0.0f, 0.0f);
    for (const auto& vertex : uniqueVertices) {
        sum += vertex;
    }
    center = sum * (1.0f / static_cast<float>(uniqueVertices.size()));
}

physx::PxVec3 MeshData::randomPointOnMesh(const std::function<double()>& distributionFunc) 
{
    if (indices.size() < 3) {
        throw std::runtime_error("Mesh does not contain enough indices to form triangles.");
    }
    // Choose a random triangle
    size_t triIndex = (rand() % (indices.size() / 3)) * 3;

    const auto& a = vertices[indices[triIndex]];
    const auto& b = vertices[indices[triIndex + 1]];
    const auto& c = vertices[indices[triIndex + 2]];

    // Random barycentric coordinates
    float u = abs(distributionFunc());
    float v = abs(distributionFunc());
    if (u + v > 1.0f) {
        u = 1.0f - u;
        v = 1.0f - v;
    }

    return a + (b - a) * u + (c - a) * v;
}

const std::vector<physx::PxVec3>& MeshData::getMeshVertices() const
{
    return uniqueVertices;
}

void MeshData::moveMeshTo(const physx::PxVec3 &position)
{
    physx::PxVec3 displacement = position - center;
    for (auto& vertex : vertices) {
        vertex += displacement;
    }
    for (auto& vertex : uniqueVertices) {
        vertex += displacement;
    }
    center = position;
}

bool MeshData::isPointInsideMesh(const physx::PxVec3 &point) const
{
    if (aabb.contains(point)) {
        return true;
    }
    return false;
}

void MeshData::computeAABB()
{
    if (uniqueVertices.empty()) {
        std::cout << "MeshData::computeAABB - No unique vertices available to compute AABB." << std::endl;
        return;
    }

    physx::PxVec3 minPt = uniqueVertices[0];
    physx::PxVec3 maxPt = uniqueVertices[0];

    for (const auto& vertex : uniqueVertices) {
        minPt.x = std::min(minPt.x, vertex.x);
        minPt.y = std::min(minPt.y, vertex.y);
        minPt.z = std::min(minPt.z, vertex.z);

        maxPt.x = std::max(maxPt.x, vertex.x);
        maxPt.y = std::max(maxPt.y, vertex.y);
        maxPt.z = std::max(maxPt.z, vertex.z);
    }

    aabb = physx::PxBounds3(minPt, maxPt);
}
