#include "SceneManager.h"

#include "scene/Scene.h"
#include "engine/render/openGL/Camera.h"


SceneManager::~SceneManager()
{
    for (auto* scene : _scenes) {
        delete scene;
        scene = nullptr;
    }
}

Scene *SceneManager::createScene()
{
    return nullptr;
}

void SceneManager::pushScene(Scene *scene)
{
    scene->init();
    _scenes.push_back(scene);
    scene->load();
}

void SceneManager::init()
{
    // Initialization code for the scene (e.g., loading models, setting up lights, etc.)
}

void SceneManager::update(float deltaTime)
{
    _scenes[_currentScene]->update();
    // Update the scene based on the elapsed time (e.g., animations, object movements, etc.)
}

void SceneManager::render(Camera* camera)
{
    _scenes[_currentScene]->render(*camera);
}

void SceneManager::shutdown()
{
    for (auto* scene : _scenes) {
        scene->unload();
        delete scene;
        scene = nullptr;
    }
    _scenes.clear();
}
