#include "PBRMaterial.h"
#include <GL/glew.h>
#include "Shader.h"
#include <iostream>

static void setSamplerUniform(GLuint program, const char* name, int unit) {
    GLint loc = glGetUniformLocation(program, name);
    if (loc >= 0) glUniform1i(loc, unit);
}

void PBRMaterial::bindAll(GLuint shaderProgram) const
{
    int unit = 0;
    if (albedo) {
        glActiveTexture(GL_TEXTURE0 + unit);
        albedo->bind();
        setSamplerUniform(shaderProgram, "uAlbedo", unit);
        ++unit;
    }
    if (normal) {
        glActiveTexture(GL_TEXTURE0 + unit);
        normal->bind();
        // std::cout << "Binding normalTex id=" << normal->getId() << std::endl;
        // std::cout << "Normal: " << (normal ? normal->getId() : 0) << std::endl;
        setSamplerUniform(shaderProgram, "uNormal", unit);
        ++unit;
    }
    // else {
    //     std::cout << "No normal texture to bind." << std::endl;
    // }
    if (metallic) {
        glActiveTexture(GL_TEXTURE0 + unit);
        metallic->bind();
        setSamplerUniform(shaderProgram, "uMetallic", unit);
        ++unit;
    }
    // else {
    //     std::cout << "No metallic texture to bind." << std::endl;
    // }
    if (roughness) {
        glActiveTexture(GL_TEXTURE0 + unit);
        roughness->bind();
        setSamplerUniform(shaderProgram, "uRoughness", unit);
        ++unit;
    }
    // else {
    //     std::cout << "No roughness texture to bind." << std::endl;
    // }
    if (ao) {
        glActiveTexture(GL_TEXTURE0 + unit);
        ao->bind();
        setSamplerUniform(shaderProgram, "uAO", unit);
        ++unit;
    }

    if (emissive) {
        glActiveTexture(GL_TEXTURE0 + unit);
        emissive->bind();
        setSamplerUniform(shaderProgram, "uEmissive", unit);
        ++unit;
    }
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