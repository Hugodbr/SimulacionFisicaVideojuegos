#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "IWindowSystem.h"
#include "GLRenderEngine.h"
#include "framework/InputManager.h"

using WindowSystemFactory = std::unique_ptr<IWindowSystem>(*)();

// Application context managing the window system and rendering API
class ApplicationContext : public IWindowEventListener {
protected:
	std::unique_ptr<IWindowSystem>  _windowSystem; // windowing system abstraction
	InputManager*				    _inputManager; // input manager
    std::unique_ptr<GLRenderEngine> _glRenderEngine; // rendering engine

public:
	static ApplicationContext* s_appInstance;
	static ApplicationContext& getInstance() { return *s_appInstance; } // static access method

	ApplicationContext(ApplicationContext const& J) = delete;   // no copy constructor
	void operator=(ApplicationContext const& J) = delete;       // no copy assignment

	virtual void init(const char* title, const std::string& windowSystem); // initialize with specified systems
	void run();   // the main event processing loop
	void close(); // the application

    int getWindowWidth() const { return _windowSystem->getWindowWidth(); }
    int getWindowHeight() const { return _windowSystem->getWindowHeight(); }

    std::string getProjectRoot() const;
    // Test
    double getAbsTime() const;

protected:
	ApplicationContext();
	virtual ~ApplicationContext();

	void destroy();

	void setWindowSystem(const std::string& windowSystem); // set window system (e.g., "GLFW")

	void registerWindowSystems();

    // Hooks for derived classes (game app)
    virtual void onInit() {}
    virtual void onUpdate(double deltaTime) {}
    virtual void onRender() {}
    virtual void onShutdown() {}
	virtual void onResize(int newWidth, int newHeight) {}

    virtual bool shouldClose();

    void setVSync(bool enabled);
    bool isVSyncEnabled() const { return _windowSystem->isVSyncEnabled(); }

private:
    // Registry for window systems
    std::unordered_map<std::string, WindowSystemFactory> _windowRegistry;

    template<typename T>
    void registerWindowSystem(const std::string& name) {
        _windowRegistry[name] = []() -> std::unique_ptr<IWindowSystem> {
            return std::make_unique<T>();
        };
    }
};