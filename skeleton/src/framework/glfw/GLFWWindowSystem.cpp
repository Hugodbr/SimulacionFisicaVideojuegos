#include "framework/glfw/GLFWWindowSystem.h"

#include <cassert>
#include <stdexcept>
#include <iostream>
#include "framework/glfw/GLFWWindowSystem.h"


bool GLFWWindowSystem::createWindow(int width, int height, const char *title)
{
    std::cout << "Starting GLFW...\n";

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    _window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!_window) {
        glfwTerminate();
        throw std::logic_error("Cannot create GLFW window");
        return false;
    }

    glfwMakeContextCurrent(_window);

    // Store 'this' pointer in window (for access in static callbacks)
    glfwSetWindowUserPointer(_window, this);
    
    // Register static callbacks
    glfwSetWindowRefreshCallback(_window, s_display);
    glfwSetWindowSizeCallback(_window, s_resize);
    glfwSetCharCallback(_window, s_key);
	glfwSetKeyCallback(_window, s_specialkey);
    glfwSetMouseButtonCallback(_window, s_mouse);
    glfwSetCursorPosCallback(_window, s_motion);
    glfwSetScrollCallback(_window, s_mouseWheel);

    return true;
}

bool GLFWWindowSystem::shouldClose() const 
{
    return glfwWindowShouldClose(_window);
}

void GLFWWindowSystem::pollEvents() 
{
    glfwPollEvents();
}

void GLFWWindowSystem::destroy() 
{
    if (_window) {
        glfwDestroyWindow(_window);
        _window = nullptr;
    }
    glfwTerminate();
}

void GLFWWindowSystem::close()
{
    std::cout << "Closing GLFW...\n";
	glfwSetWindowShouldClose(_window, true); // stops main loop
    std::cout << "GLFW closed.\n";
}

void* GLFWWindowSystem::getNativeHandle() 
{
    return static_cast<void*>(_window);
}

void GLFWWindowSystem::setVSync(bool enabled)
{
    glfwSwapInterval(enabled ? 1 : 0);
    _VSyncEnabled = enabled;
}

bool GLFWWindowSystem::isVSyncEnabled() const
{
    return _VSyncEnabled;
}

// =================================================================================================
// Static Callbacks
// =================================================================================================
void GLFWWindowSystem::s_display(GLFWwindow* win)
{
    GLFWWindowSystem* windowSystem = static_cast<GLFWWindowSystem*>(glfwGetWindowUserPointer(win));
    assert(windowSystem);
    windowSystem->display();
}

void GLFWWindowSystem::s_resize(GLFWwindow *win, int newWidth, int newHeight)
{
    GLFWWindowSystem* windowSystem = static_cast<GLFWWindowSystem*>(glfwGetWindowUserPointer(win));
    assert(windowSystem);
    windowSystem->resize(newWidth, newHeight);
}

void GLFWWindowSystem::s_key(GLFWwindow* win, unsigned int codepoint)
{
    GLFWWindowSystem* windowSystem = static_cast<GLFWWindowSystem*>(glfwGetWindowUserPointer(win));
    assert(windowSystem);
    windowSystem->key(codepoint);
}

void GLFWWindowSystem::s_specialkey(GLFWwindow* win, int key, int scancode, int action, int mods)
{
    GLFWWindowSystem* windowSystem = static_cast<GLFWWindowSystem*>(glfwGetWindowUserPointer(win));
    assert(windowSystem);
    windowSystem->specialkey(key, scancode, action, mods);
}

void GLFWWindowSystem::s_mouse(GLFWwindow* win, int button, int action, int mods)
{
    GLFWWindowSystem* windowSystem = static_cast<GLFWWindowSystem*>(glfwGetWindowUserPointer(win));
    assert(windowSystem);
    windowSystem->mouse(button, action, mods);
}

void GLFWWindowSystem::s_motion(GLFWwindow* win, double x, double y)
{
    GLFWWindowSystem* windowSystem = static_cast<GLFWWindowSystem*>(glfwGetWindowUserPointer(win));
    assert(windowSystem);
    windowSystem->motion(x, y);
}

void GLFWWindowSystem::s_mouseWheel(GLFWwindow* win, double dx, double dy)
{
    GLFWWindowSystem* windowSystem = static_cast<GLFWWindowSystem*>(glfwGetWindowUserPointer(win));
    assert(windowSystem);
    windowSystem->mouseWheel(dx, dy);
}


// =================================================================================================
// Member functions called by static callbacks
// Forward events to registered listeners through IWindowSystem and IInputEventListener interfaces
// =================================================================================================
void GLFWWindowSystem::display()
{
    for (auto* listener : _windowEventListeners) {
        listener->onDisplay();
    }

    glfwSwapBuffers(_window); // swaps the front and back buffer
}

void GLFWWindowSystem::resize(int newWidth, int newHeight)
{
    _width = newWidth;
    _height = newHeight;

    for (auto* listener : _windowEventListeners) {
        listener->onResize(newWidth, newHeight);
    }
}

void GLFWWindowSystem::key(unsigned int codepoint)
{
    for (auto* listener : _inputEventListeners) {
        listener->onCharEvent(codepoint);
    }
}

void GLFWWindowSystem::specialkey(int key, int scancode, int action, int mods)
{
    for (auto* listener : _inputEventListeners) {
        listener->onKeyEvent(mapGLFWKey(key), scancode, mapGLFWKeyAction(action), mapGLFWKeyMod(mods));
    }
}

void GLFWWindowSystem::mouse(int button, int action, int mods)
{
    for (auto* listener : _inputEventListeners) {
        listener->onMouseButton(mapGLFWMouseButton(button), mapGLFWKeyAction(action), mapGLFWKeyMod(mods));
    }
}

void GLFWWindowSystem::motion(double x, double y)
{
    for (auto* listener : _inputEventListeners) {
        listener->onMouseMove(x, y);
    }
}

void GLFWWindowSystem::mouseWheel(double dx, double dy)
{
    for (auto* listener : _inputEventListeners) {
        listener->onMouseScroll(dx, dy);
    }
}

KeyCode GLFWWindowSystem::mapGLFWKey(int glfwKey)
{
    switch (glfwKey)
    {
    case GLFW_KEY_A:
        return KeyCode::A;
    case GLFW_KEY_B:
        return KeyCode::B;
    case GLFW_KEY_C:
        return KeyCode::C;
    case GLFW_KEY_D:
        return KeyCode::D;
    case GLFW_KEY_E:
        return KeyCode::E;
    case GLFW_KEY_F:
        return KeyCode::F;
    case GLFW_KEY_G:
        return KeyCode::G;
    case GLFW_KEY_H:
        return KeyCode::H;
    case GLFW_KEY_I:
        return KeyCode::I;
    case GLFW_KEY_J:
        return KeyCode::J;
    case GLFW_KEY_K:
        return KeyCode::K;
    case GLFW_KEY_L:
        return KeyCode::L;
    case GLFW_KEY_M:
        return KeyCode::M;
    case GLFW_KEY_N:
        return KeyCode::N;
    case GLFW_KEY_O:    
        return KeyCode::O;
    case GLFW_KEY_P:
        return KeyCode::P;
    case GLFW_KEY_Q:
        return KeyCode::Q;
    case GLFW_KEY_R:
        return KeyCode::R;
    case GLFW_KEY_S:
        return KeyCode::S;
    case GLFW_KEY_T:
        return KeyCode::T;
    case GLFW_KEY_U:
        return KeyCode::U;  
    case GLFW_KEY_V:
        return KeyCode::V;
    case GLFW_KEY_W:
        return KeyCode::W;
    case GLFW_KEY_X:
        return KeyCode::X;
    case GLFW_KEY_Y:
        return KeyCode::Y;
    case GLFW_KEY_Z:
        return KeyCode::Z;
    case GLFW_KEY_ESCAPE:
        return KeyCode::Escape;
    case GLFW_KEY_ENTER:
        return KeyCode::Enter;
    case GLFW_KEY_TAB:
        return KeyCode::Tab;
    case GLFW_KEY_BACKSPACE:
        return KeyCode::Backspace;
    case GLFW_KEY_SPACE:
        return KeyCode::Space;
    case GLFW_KEY_CAPS_LOCK:
        return KeyCode::CapsLock;
    case GLFW_KEY_LEFT_SHIFT:
        return KeyCode::ShiftLeft;
    case GLFW_KEY_RIGHT_SHIFT:
        return KeyCode::ShiftRight;
    case GLFW_KEY_LEFT_CONTROL:
        return KeyCode::CtrlLeft;
    case GLFW_KEY_RIGHT_CONTROL:
        return KeyCode::CtrlRight;
    case GLFW_KEY_LEFT_ALT:
        return KeyCode::AltLeft;
    case GLFW_KEY_RIGHT_ALT:
        return KeyCode::AltRight;
    case GLFW_KEY_UP:
        return KeyCode::ArrowUp;
    case GLFW_KEY_DOWN:
        return KeyCode::ArrowDown;
    case GLFW_KEY_LEFT:
        return KeyCode::ArrowLeft;
    case GLFW_KEY_RIGHT:
        return KeyCode::ArrowRight;    
    default:
        break;
    }
}

MouseButton GLFWWindowSystem::mapGLFWMouseButton(int glfwButton)
{
    switch (glfwButton)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
        return MouseButton::Left;
    case GLFW_MOUSE_BUTTON_RIGHT:
        return MouseButton::Right;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        return MouseButton::Middle;
    }
}

KeyAction GLFWWindowSystem::mapGLFWKeyAction(int glfwAction)
{
    switch (glfwAction)
    {
    case GLFW_PRESS:
        return KeyAction::Press;
    case GLFW_RELEASE:
        return KeyAction::Release;
    case GLFW_REPEAT:
        return KeyAction::Repeat;
    default:
        return KeyAction::Press; // Default case
    }
}

KeyMod GLFWWindowSystem::mapGLFWKeyMod(int glfwMods)
{
    KeyMod mods = KeyMod::None;

    if (glfwMods & GLFW_MOD_SHIFT) {
        mods = static_cast<KeyMod>(static_cast<uint8_t>(mods) | static_cast<uint8_t>(KeyMod::Shift));
    }
    if (glfwMods & GLFW_MOD_CONTROL) {
        mods = static_cast<KeyMod>(static_cast<uint8_t>(mods) | static_cast<uint8_t>(KeyMod::Ctrl));
    }
    if (glfwMods & GLFW_MOD_ALT) {
        mods = static_cast<KeyMod>(static_cast<uint8_t>(mods) | static_cast<uint8_t>(KeyMod::Alt));
    }

    return mods;
}