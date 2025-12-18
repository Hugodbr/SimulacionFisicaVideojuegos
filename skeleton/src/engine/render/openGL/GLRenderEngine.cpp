#include "render/openGL/GLRenderEngine.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "framework/IWindowSystem.h"

#include <iostream>


// Print OpenGL errors and warnings
void GLAPIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                   GLsizei length, const GLchar* message, const void* userParam)
{
	const char* prefix = (type == GL_DEBUG_TYPE_ERROR)
		? "\x1b[31m[ERROR]\x1b[0m "
		: "\x1b[33m[WARNING]\x1b[0m ";
	std::cout << prefix << message << std::endl;
}

GLRenderEngine::GLRenderEngine()
{
    _windowHandle = nullptr;
    _initialized = false;
}

GLRenderEngine::~GLRenderEngine()
{
    shutdown();
}

bool GLRenderEngine::initialize(void *windowHandle)
{
    // Initialize GLEW or any other OpenGL related setup here
    std::cout << "Initializing OpenGL...\n";
    _windowHandle = windowHandle;
    
    std::cout << "Initializing GLEW...\n";
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error initializing GLEW: " << glewGetErrorString(err) << std::endl;
        return false;
    }

    // Error message callback (all messages)
	// glEnable(GL_DEBUG_OUTPUT);
	// glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0u, 0, GL_TRUE);
	// glDebugMessageCallback(debugCallback, nullptr);
    glDisable(GL_DEBUG_OUTPUT);

	std::cout << "Version: " << glGetString(GL_VERSION) << '\n';
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << '\n';

    _initialized = true;
    return true;
}

void GLRenderEngine::beginFrame() 
{
    // background
    glClearColor(0.733f, 0.871f, 0.984f, 1.0f);
    // double buffering
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears the back buffer
}

void GLRenderEngine::endFrame() 
{
    // Swap buffers would typically be handled by the windowing system
    // Here we assume that the window handle has a method to swap buffers
    // static_cast<GLFWwindow*>(window->getNativeHandle())->SwapBuffers();
}

void GLRenderEngine::present()
{
    glfwSwapBuffers(static_cast<GLFWwindow*>(_windowHandle)); // swaps the front and back buffer
}

void GLRenderEngine::clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRenderEngine::shutdown()
{
    std::cout << "Shutting down OpenGL...\n";
    // Any necessary cleanup for OpenGL
    _initialized = false;
}

void GLRenderEngine::setVSync(bool enabled)
{
    static_cast<IWindowSystem*>(_windowHandle)->setVSync(enabled);
}