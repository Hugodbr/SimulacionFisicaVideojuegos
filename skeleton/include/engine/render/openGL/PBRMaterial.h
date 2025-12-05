#pragma once

#include <string>
#include <memory>
#include "Texture.h"

struct PBRMaterial {
    // Keep ownership simple: raw pointers (nullptr if not present).
    // You may later switch to unique_ptr/shared_ptr if desired.
    Texture* albedo = nullptr;     // aiTextureType_DIFFUSE
    Texture* normal = nullptr;     // aiTextureType_NORMALS / aiTextureType_NORMAL_CAMERA
    Texture* metallic = nullptr;   // aiTextureType_METALNESS or aiTextureType_UNKNOWN
    Texture* roughness = nullptr;  // aiTextureType_DIFFUSE_ROUGHNESS
    Texture* ao = nullptr;         // aiTextureType_AMBIENT_OCCLUSION or aiTextureType_LIGHTMAP
    Texture* emissive = nullptr;   // aiTextureType_EMISSIVE

    // helper: bind to consecutive texture units and set sampler uniform names
    void bindAll(GLuint shaderProgram) const;
    void unbindAll() const;
};