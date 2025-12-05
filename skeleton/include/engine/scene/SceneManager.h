#pragma once

#include <vector>

class Scene;
class Camera;

class SceneManager {

public:
    static SceneManager& getInstance()
    {
        static SceneManager instance;
        return instance;
    }

    Scene* createScene();
    void pushScene(Scene* scene);
    Scene* getCurrentScene() const { return _scenes[_currentScene]; }

    void init();
    void update(float deltaTime);
    void render(Camera* camera);
    void shutdown();

private:
    SceneManager() = default;
    ~SceneManager();

    std::vector<Scene*> _scenes;
	size_t _currentScene = 0;
};