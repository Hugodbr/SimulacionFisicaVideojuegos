#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "PxPhysicsAPI.h"


struct MeshData {
    std::vector<physx::PxVec3> vertices;
    std::vector<unsigned int> indices;
};
