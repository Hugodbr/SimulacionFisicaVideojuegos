#include "ApplicationContext.h"

#include <GL/glew.h>     // OpenGL Extension Wrangler
#include <GLFW/glfw3.h>  // OpenGL Library
#include <glm/glm.hpp>   // OpenGL Mathematics

#include <iostream>
#include <stdexcept>

#include "glfw/GLFWWindowSystem.h"
#include "core/ResourceLocator.h"


ApplicationContext* ApplicationContext::s_appInstance = nullptr;

double ApplicationContext::getAbsTime() const
{
    return glfwGetTime();
}

ApplicationContext::ApplicationContext()
{
    s_appInstance = this;
}

ApplicationContext::~ApplicationContext() {
    s_appInstance = nullptr;
}

void ApplicationContext::init(const char* title, const std::string& windowSystem)
{
	std::cout << "Initializing Application...\n";

	std::cout << "Setting project root...\n";
	auto projectRoot = findProjectRoot();
	ResourceLocator::setProjectRoot(projectRoot);
	std::cout << "Project root set to: " << projectRoot << "\n";

	std::cout << "Setting up window system (" << windowSystem << ")...\n";
	registerWindowSystems();
	setWindowSystem(windowSystem);
	// Create window
	if (!_windowSystem->createWindow(800, 600, title)) {
		throw std::runtime_error("Failed to create window using " + windowSystem);
	}

	std::cout << "Setting up render API...\n";
	_glRenderEngine = std::make_unique<GLRenderEngine>();
	if (!_glRenderEngine->initialize(_windowSystem->getNativeHandle())) {
		throw std::runtime_error("Failed to initialize render API");
	}

	std::cout << "Setting up input manager and listeners...\n";
	_inputManager = &InputManager::getInstance();
	_windowSystem->addWindowEventListener(this);
	_windowSystem->addInputEventListener(_inputManager);


	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // enable on macOS
}

void ApplicationContext::setWindowSystem(const std::string &windowSystem)
{
	auto it = _windowRegistry.find(windowSystem);

	if (it != _windowRegistry.end()) {
		_windowSystem = it->second();
	} 
	else {
		throw std::logic_error("Unknown window system: " + windowSystem);
		std::cout << "Available window systems:\n";
		for (const auto& pair : _windowRegistry) {
			std::cout << " - " << pair.first << '\n';
		}
	}
}

void ApplicationContext::run() // enters the main event processing loop
{
    onInit(); // hook for derived classes (game app)

    double lastTime = glfwGetTime();

	// ApplicationContext main loop
    while (!shouldClose()) {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

		_inputManager->beginFrame();
		_windowSystem->pollEvents();

        onUpdate(deltaTime); // hook for derived classes (game app)

		_glRenderEngine->beginFrame();
        onRender(); // hook for derived classes (game app)
		_glRenderEngine->endFrame();
		_glRenderEngine->present();
    }

    onShutdown(); // hook for derived classes (game app)
	close();
	destroy();
}

bool ApplicationContext::shouldClose()
{
    return _windowSystem->shouldClose();
}

void ApplicationContext::close()
{
	_windowSystem->close();
    std::cout << "Application closed.\n";
}

void ApplicationContext::destroy() 
{
	_windowSystem->destroy();
}

void ApplicationContext::registerWindowSystems()
{
	registerWindowSystem<GLFWWindowSystem>("GLFW");
	// Future window systems can be registered here
}

void ApplicationContext::setVSync(bool enabled)
{
    _windowSystem->setVSync(enabled);
}
