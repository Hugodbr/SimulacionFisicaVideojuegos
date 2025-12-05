#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "IWindowSystem.h"
#include "framework/InputKeys.h"


// GLFW implementation of the WindowSystem interface
class GLFWWindowSystem : public IWindowSystem {
private:
    GLFWwindow* _window = nullptr;

    // IWindowSystem interface implementation
public:
    virtual bool  createWindow(int width, int height, const char* title) override;
    virtual bool  shouldClose() const override;
    virtual void  pollEvents() override;
    virtual void  destroy() override;
	virtual void  close() override;
    virtual void* getNativeHandle() override; // returns GLFWwindow*

	virtual void setVSync(bool enabled) override;
    virtual bool isVSyncEnabled() const override;

private:
	// Static callbacks (GLFW requires static functions as callbacks because of C linkage)
	static void s_display(GLFWwindow* win);
	static void s_resize(GLFWwindow* win, int newWidth, int newHeight);
	static void s_key(GLFWwindow* win, unsigned int codepoint);
	static void s_specialkey(GLFWwindow* win, int key, int scancode, int action, int mods);
	static void s_mouse(GLFWwindow* win, int button, int action, int mods);
	static void s_motion(GLFWwindow* win, double x, double y);
	static void s_mouseWheel(GLFWwindow* win, double dx, double dy);

	// Member functions called by static callbacks
	void display();
	void resize(int newWidth, int newHeight);  // the viewport (without changing the scale)
	void key(unsigned int codepoint); // keypress event
	void specialkey(int key, int scancode, int action, int mods); // special keypress event
	void mouse(int button, int action, int mods);
	void motion(double x, double y);
	void mouseWheel(double dx, double dy);

    KeyCode mapGLFWKey(int glfwKey);
    MouseButton mapGLFWMouseButton(int glfwButton);
    KeyAction mapGLFWKeyAction(int glfwAction);
    KeyMod mapGLFWKeyMod(int glfwMods);
};
