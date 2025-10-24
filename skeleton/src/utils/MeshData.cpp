#include "MeshData.h"

MeshData loadMesh(const std::string& filename) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename,
        aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

    MeshData mesh;
    if (!scene || !scene->HasMeshes()) return mesh;

    aiMesh* m = scene->mMeshes[0];
    mesh.vertices.reserve(m->mNumVertices);
    for (unsigned int i = 0; i < m->mNumVertices; ++i)
        mesh.vertices.emplace_back(m->mVertices[i].x, m->mVertices[i].y, m->mVertices[i].z);

    for (unsigned int i = 0; i < m->mNumFaces; ++i)
        for (unsigned int j = 0; j < m->mFaces[i].mNumIndices; ++j)
            mesh.indices.push_back(m->mFaces[i].mIndices[j]);

    return mesh;
}