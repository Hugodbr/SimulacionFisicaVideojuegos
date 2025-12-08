#include "PBRMaterial.h"
#include <GL/glew.h>
#include "Shader.h"
#include <iostream>

static void setSamplerUniform(GLuint program, const char* name, int unit) {
    GLint loc = glGetUniformLocation(program, name);
    if (loc >= 0) glUniform1i(loc, unit);
}

void PBRMaterial::bindAll(GLuint shaderProgram)
{
    hasAlbedoTex = hasNormalTex = hasMetallicTex = hasRoughnessTex = hasAOTex = hasEmissiveTex = hasMetallicRoughnessTex = false;

    int unit = 0;

    if (albedoTex) {
        glActiveTexture(GL_TEXTURE0 + unit);
        albedoTex->bind();
        setSamplerUniform(shaderProgram, "uAlbedo", unit);
        ++unit;
        hasAlbedoTex = true;
    }

    if (normalTex) {
        glActiveTexture(GL_TEXTURE0 + unit);
        normalTex->bind();
        // std::cout << "Binding normalTex id=" << normalTex->getId() << std::endl;
        // std::cout << "Normal: " << (normalTex ? normalTex->getId() : 0) << std::endl;
        setSamplerUniform(shaderProgram, "uNormal", unit);
        ++unit;
        hasNormalTex = true;
    }

    if(metallicRoughnessTex) {
        glActiveTexture(GL_TEXTURE0 + unit);
        metallicRoughnessTex->bind();
        setSamplerUniform(shaderProgram, "uMetallicRoughness", unit);
        ++unit;
        hasMetallicRoughnessTex = true;
    }

    // else {
    //     std::cout << "No normal texture to bind." << std::endl;
    // }
    if (metallicTex) {
        glActiveTexture(GL_TEXTURE0 + unit);
        metallicTex->bind();
        setSamplerUniform(shaderProgram, "uMetallic", unit);
        ++unit;
        hasMetallicTex = true;
    }
    // else {
    //     std::cout << "No metallic texture to bind." << std::endl;
    // }
    if (roughnessTex) {
        glActiveTexture(GL_TEXTURE0 + unit);
        roughnessTex->bind();
        setSamplerUniform(shaderProgram, "uRoughness", unit);
        ++unit;
        hasRoughnessTex = true;
    }
    // else {
    //     std::cout << "No roughness texture to bind." << std::endl;
    // }
    if (aoTex) {
        glActiveTexture(GL_TEXTURE0 + unit);
        aoTex->bind();
        setSamplerUniform(shaderProgram, "uAO", unit);
        ++unit;
        hasAOTex = true;
    }

    if (emissiveTex) {
        glActiveTexture(GL_TEXTURE0 + unit);
        emissiveTex->bind();
        setSamplerUniform(shaderProgram, "uEmissive", unit);
        ++unit;
        hasEmissiveTex = true;
    }

    glUniform1i(glGetUniformLocation(shaderProgram, "hasAlbedoTex"), hasAlbedoTex);
    glUniform1i(glGetUniformLocation(shaderProgram, "hasMetalTex"), hasMetallicTex);
    glUniform1i(glGetUniformLocation(shaderProgram, "hasRoughTex"), hasRoughnessTex);
    glUniform1i(glGetUniformLocation(shaderProgram, "hasNormalTex"), hasNormalTex);
    glUniform1i(glGetUniformLocation(shaderProgram, "hasAoTex"), hasAOTex);
    glUniform1i(glGetUniformLocation(shaderProgram, "hasEmissiveTex"), hasEmissiveTex);
    glUniform1i(glGetUniformLocation(shaderProgram, "hasMetallicRoughnessTex"), hasMetallicRoughnessTex);

    glUniform3fv(glGetUniformLocation(shaderProgram, "uBaseColor"), 1, &albedoColor[0]);
    glUniform1f(glGetUniformLocation(shaderProgram, "uMetallicValue"), metallicValue);
    glUniform1f(glGetUniformLocation(shaderProgram, "uRoughnessValue"), roughnessValue);
    glUniform1f(glGetUniformLocation(shaderProgram, "uAoValue"), aoValue);
    glUniform3fv(glGetUniformLocation(shaderProgram, "uEmissiveColor"), 1, &emissiveColor[0]);
}

void PBRMaterial::unbindAll() const
{
    // Unbind all used texture units (simple approach)
    // Note: if you allocate many textures per frame you may want to track count
    const int maxUnits = 8;
    for (int i = 0; i < maxUnits; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}