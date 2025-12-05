#ifndef _H_Scene_H_
#define _H_Scene_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Camera.h"
#include "EntityRenderable.h"
#include "Light.h"

#include <vector>


enum {
	OFF,
	ON
};

class Scene
{
public:
	Scene() = default;
	~Scene();

	Scene(const Scene& s) = delete;            // no copy constructor
	Scene& operator=(const Scene& s) = delete; // no copy assignment

	virtual void init();

	void render(Camera const& cam) const;
	virtual void update() {};

	// load or unload scene data into the GPU
	void load();
	void unload();

	void uploadLights(Camera const& cam) const;

	virtual void printscreen() {};
	virtual void showNormals();

	virtual void rotate() {}
	virtual void orbit() {}

	void toggleLight(Light* light);

	void toggleLightWithKey_R(); // Global. All scenes
	virtual void toggleLightWithKey_T() {}
	virtual void toggleLightWithKey_Y() {}
	virtual void toggleLightWithKey_H() {}

	int getDirLightID();
	int getSpotLightID();
	int getPosLightID();

protected:
	void destroy();
	// void setGL();
	// void resetGL();
	// virtual void setColor();

	std::vector<Abs_Entity*> gObjects; // Entities (graphic objects) of the scene
	std::vector<Abs_Entity*> gTranslucidObjs; // Entities (translucid objects) of the scene
	std::vector<Texture*> gTextures; // Texturas de la escena
	std::vector<std::pair<Light*, bool>> gLights; // Luces de la escena: {light, on/off}

	// Max number of lights for each type (capped by shader)
	const int NR_DIR_LIGHTS = 2;
	const int NR_POS_LIGHTS = 4;
	const int NR_SPOT_LIGHTS = 4;

	// Current number of lights
	int nDirLights = 0;
	int nSpotLights = 0;
	int nPosLights = 0;

	DirLight* mGlobalLight = nullptr;

	bool mShowNormals = false;
};

#endif //_H_Scene_H_
