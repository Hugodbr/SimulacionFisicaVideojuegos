#ifndef _H_Light_H
#define _H_Light_H

#include <string>

#include "Shader.h"
#include <glm/glm.hpp>

class Light { // Abstract class
public:
	virtual ~Light();

	bool enabled() const { return bEnabled; }
	void setEnabled(bool enabled) { bEnabled = enabled; }

	virtual void upload(Shader& shader, glm::mat4 const& modelViewMat) const;

	void unload(Shader& shader);

	void setAmb(const glm::vec3& ind);
	void setDiff(const glm::vec3& ind);
	void setSpec(const glm::vec3& ind);

protected:
	Light(std::string name);
	Light(const std::string& name, int id);

	// Identifier
	std::string lightID;
	bool bEnabled;

	// Atributos lumínicos y geométrico de la fuente de luz
	//glm::vec3 ambient = {0.1, 0.1, 0.1};
	//glm::vec3 diffuse = {0.5, 0.5, 0.5};
	//glm::vec3 specular = {0.5, 0.5, 0.5};

	glm::vec3 ambient = { 0.3f, 0.3f, 0.3f }; // Brighter base even in shadow
	glm::vec3 diffuse = { 1.0f, 1.0f, 1.0f }; // Full white diffuse light
	glm::vec3 specular = { 1.0f, 1.0f, 1.0f }; // For strong highlights
};

class DirLight : public Light {
public:
	explicit DirLight(int id = 0);

	virtual void upload(Shader& shader, glm::mat4 const& modelViewMat) const override;
	void setDirection(const glm::vec3& dir);

protected:
	glm::vec4 direction = {-1, -1, -1, 0};
};

inline void
DirLight::setDirection(const glm::vec3& dir) {
	direction = glm::vec4(dir, 0.0);
}

class PosLight : public Light {
public:
	explicit PosLight(int id = 0);

	virtual void upload(Shader& shader, glm::mat4 const& modelViewMat) const override;

	void setPosition(const glm::fvec3& dir);
	const glm::vec3& getPosition();

	void setAttenuation(GLfloat kc, GLfloat kl, GLfloat kq);

protected:
	glm::vec4 position = {0, 0, 0, 1};
	// Factores de atenuación
	GLfloat constant = 1, linear = 0, quadratic = 0;
};

inline void
PosLight::setPosition(const glm::vec3& pos) {
	position = glm::vec4(pos, 1.0);
}

inline
const glm::vec3& PosLight::getPosition() {
	return position;
}

inline void
PosLight::setAttenuation(GLfloat nkc, GLfloat nkl, GLfloat nkq) {
	constant = nkc;
	linear = nkl;
	quadratic = nkq;
}

class SpotLight : public PosLight {
public:
	SpotLight(int id = 0, const glm::vec3& pos = { 0, 0, 0 });

	virtual void upload(Shader& shader, glm::mat4 const& modelViewMat) const override;
	void setDirection(const glm::vec3& dir) { direction = dir; }
	void setCutoff(float inner, float outer);

protected:
	// Atributos del foco
	glm::vec3 direction = {0, 0, -1};
	GLfloat cutoff = 60, outerCutoff = 120;
	GLfloat exp = 0; // no sé si se usa
};


// ADDED LIGHTS

class DirLightSimple : public DirLight
{
public:
	explicit DirLightSimple(int id = 0);
};

class PosLightXY : public PosLight
{
public:
	explicit PosLightXY(int id = 0);
};

class SpotLightYZ : public SpotLight
{
public:
	explicit SpotLightYZ(int id = 0, const glm::vec3& pos = { 0, 0, 0 });
};


#endif //_H_Light_H_