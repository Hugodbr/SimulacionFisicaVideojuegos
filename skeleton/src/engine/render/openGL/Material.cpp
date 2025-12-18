#include "Material.h"

#include <glm/gtc/type_ptr.hpp>

using namespace glm;

Material::Material(vec3 color, float shininess)
 : ambient(color)
 , diffuse(color)
 , specular(color)
 , expF(shininess)
{
}

void
Material::upload(Shader& lighting) const
{
	lighting.setUniform("material.ambient", ambient);
	lighting.setUniform("material.diffuse", diffuse);
	lighting.setUniform("material.specular", specular);
	lighting.setUniform("material.shininess", expF);

	// glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	// can be simulated with a Boolean uniform and
	// gl_FrontFacing in the fragment shader
}

void
Material::setCopper()
{
	ambient = {0.19125, 0.0735, 0.0225};
	diffuse = {0.7038, 0.27048, 0.0828};
	specular = {0.256777, 0.137622, 0.086014};
	expF = 12.8;
}

void Material::setGold()
{
    ambient = { 0.24725f, 0.1995f, 0.0745f };
    diffuse = { 0.75164f, 0.60648f, 0.22648f };
    specular = { 0.628281f, 0.555802f, 0.366065f };
    expF = 51.2f;
}

void Material::setSilver()
{
    ambient = { 0.19225f, 0.19225f, 0.19225f };
    diffuse = { 0.50754f, 0.50754f, 0.50754f };
    specular = { 0.508273f, 0.508273f, 0.508273f };
    expF = 51.2f;
}

void Material::setEmerald()
{
    ambient = { 0.0215f, 0.1745f, 0.0215f };
    diffuse = { 0.07568f, 0.61424f, 0.07568f };
    specular = { 0.633f, 0.727811f, 0.633f };
    expF = 76.8f;
}

void Material::setRuby()
{
    ambient = { 0.1745f, 0.01175f, 0.01175f };
    diffuse = { 0.61424f, 0.04136f, 0.04136f };
    specular = { 0.727811f, 0.626959f, 0.626959f };
    expF = 76.8f;
}

void Material::setPearl()
{
    ambient = { 0.25f, 0.20725f, 0.20725f };
    diffuse = { 1.0f, 0.829f, 0.829f };
    specular = { 0.296648f, 0.296648f, 0.296648f };
    expF = 11.264f;
}

void Material::setPlasticRed()
{
    ambient = { 0.0f, 0.0f, 0.0f };
    diffuse = { 0.5f, 0.0f, 0.0f };
    specular = { 0.7f, 0.6f, 0.6f };
    expF = 32.0f;
}

void Material::setObsidian()
{
    ambient = { 0.05375f, 0.05f, 0.06625f };
    diffuse = { 0.18275f, 0.17f, 0.22525f };
    specular = { 0.332741f, 0.328634f, 0.346435f };
    expF = 38.4f;
}

void Material::setOceanBlue()
{
    ambient = { 0.0f, 0.1f, 0.2f };
    diffuse = { 0.0f, 0.5f, 0.7f };
    specular = { 0.5f, 0.7f, 0.9f };
    expF = 25.0f;
}

void Material::setTurquoise()
{
    ambient = { 0.1f, 0.3f, 0.3f };
    diffuse = { 0.0f, 0.5f, 0.5f };
    specular = { 0.5f, 0.7f, 0.7f };
    expF = 32.0f;
}

void Material::setSeaBlue()
{
    ambient = { 0.0f, 0.2f, 0.3f };
    diffuse = { 0.0f, 0.4f, 0.6f };
    specular = { 0.3f, 0.5f, 0.7f };
    expF = 20.0f;
}
