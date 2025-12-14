#include "Scene.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "GameApp.h"

using namespace glm;

void
Scene::init()
{
	ROOT_DIR = GameApp::getInstance().getProjectRoot();
	std::cout << "Scene root directory set to: " << ROOT_DIR << std::endl;

	// allocate memory and load resources
	// Lights
	mGlobalLight = new DirLight(getDirLightID());
	mGlobalLight->setDirection(glm::vec3(-1, -1, -1));
	gLights.push_back({ mGlobalLight, ON });
	
	// gLights.push_back({ new SpotLightYZ(getSpotLightID()), ON });
}

Scene::~Scene()
{
	destroy();
}

void
Scene::destroy()
{
	for (auto& [light, state] : gLights)
		delete light;

	gLights.clear();

	_particleSystems.clear();
	_rigidBodySystems.clear();
	_rigidBodies.clear();
}

void
Scene::load()
{
	for (const auto& ps : _particleSystems) {
		ps->load();
	}

	for (const auto& rbs : _rigidBodySystems) {
		rbs->load();
	}

	for (const auto& rb : _rigidBodies) {
		rb->load();
	}
}

void
Scene::unload()
{
	for (const auto& ps : _particleSystems) {
		ps->unload();
	}

	for (const auto& rbs : _rigidBodySystems) {
		rbs->unload();
	}

	for (const auto& rb : _rigidBodies) {
		rb->unload();
	}

	for (auto& [light, state] : gLights)
		light->unload(*Shader::get("light"));
}

void Scene::uploadLights(Camera const& cam) const
{
	Shader* s = Shader::get("light"); // We get the shader: a pointer to the shader "light" so we can use it = set as a context to setUniforms = upload uniform data
	s->use();

	//for(Abs_Entity* e : gObjects) // Objects don't have light. Light interact with objects, but are "owned" by the scene. 
	// ! Lights are used to set shader parameters !
	for (auto& [light, state] : gLights) {
		light->upload(*s, cam.viewMat()); // We upload the shader
	}

}

void Scene::toggleLight(Light* light)
{
	// Searches for the light requested to turn ON or OFF
	for (auto& [plight, pstate] : gLights) {
		if (plight == light) {// change light state ON/OFF
			pstate = !pstate;
			plight->setEnabled(pstate);
			return;
		}
	}
}

void Scene::toggleLightWithKey_R()
{
	toggleLight(mGlobalLight);
}

int Scene::getDirLightID()
{
	assert(nDirLights < NR_DIR_LIGHTS && "DIR light capacity exceeded!");

	int aux = nDirLights;
	nDirLights++;
	return aux;
}

int Scene::getSpotLightID()
{
	assert(nSpotLights < NR_SPOT_LIGHTS && "SPOT light capacity exceeded!");

	int aux = nSpotLights;
	nSpotLights++;
	return aux;
}

int Scene::getPosLightID()
{
	assert(nPosLights < NR_POS_LIGHTS && "POS light capacity exceeded!");

	int aux = nPosLights;
	nPosLights++;
	return aux;
}

void
Scene::render(Camera const& cam) const
{
	cam.upload();
	uploadLights(cam); // see this later

	for (const auto& ps : _particleSystems) {
		ps->render(cam.viewMat());
	}

	for (const auto& rbs : _rigidBodySystems) {
		rbs->render(cam.viewMat());
	}

	for (const auto& rb : _rigidBodies) {
		rb->render(cam.viewMat());
	}
}
