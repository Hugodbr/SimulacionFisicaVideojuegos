#include "MeshData.h"

#include <stdexcept>
#include <random>


void MeshData::loadMesh(const std::string& filename) 
{
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