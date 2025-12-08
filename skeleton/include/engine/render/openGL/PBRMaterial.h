#pragma once

#include <string>
#include <memory>
#include "Texture.h"

struct PBRMaterial {
    //
    // 1. Optional textures
    //
    Texture* albedoTex    = nullptr;
    Texture* normalTex    = nullptr;
    
    // glTF metallic-roughness combined texture
    Texture* metallicRoughnessTex = nullptr;

    Texture* metallicTex  = nullptr;   // FBX fallback
    Texture* roughnessTex = nullptr;   // FBX fallback

    Texture* aoTex        = nullptr;
    Texture* emissiveTex  = nullptr;

    //
    // 2. Flags indicating texture availability
    //
    bool hasAlbedoTex    = false;
    bool hasNormalTex    = false;
    bool hasMetallicTex  = false;
    bool hasRoughnessTex = false;
    bool hasAOTex        = false;
    bool hasEmissiveTex  = false;
    bool hasMetallicRoughnessTex = false;

    //
    // 3. Scalar fallback constants
    //
    glm::vec3 albedoColor   = glm::vec3(1.0f);
    float metallicValue     = 0.0f;
    float roughnessValue    = 0.8f;
    float aoValue           = 1.0f;
    glm::vec3 emissiveColor = glm::vec3(0.0f);

    // helper: bind to consecutive texture units and set sampler uniform names
    void bindAll(GLuint shaderProgram);
    void unbindAll() const;
};