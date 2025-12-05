#pragma once

#include <vector>

#include "IWindowEventListener.h"
#include "IInputEventListener.h"


// Abstract interface for window system (strict interface, must implement all methods)
// This allows for different windowing backends (e.g., GLFW, SDL)
// - Window creation, management and destruction
// - Event polling and dispatching(=delivering as events happen, immediately)
// - Access to native window handle for rendering API integration
// - Translation of raw input events to higher-level events
// - Management of window event listeners
// Is an event source for window and input events.
class IWindowSystem {
public:    
    virtual ~IWindowSystem() = default;

    virtual bool  createWindow(int width, int height, const char* title) = 0;
    virtual bool  shouldClose() const = 0;
    virtual void  pollEvents() = 0;
    virtual void  destroy() = 0;
    virtual void  close() { } // optional close method
    virtual void* getNativeHandle() = 0;

    virtual void setVSync(bool enabled) = 0;
    virtual bool isVSyncEnabled() const = 0;

    int getWindowWidth() const { return _width; }
    int getWindowHeight() const { return _height; }

    void addWindowEventListener(IWindowEventListener* listener) {
        _windowEventListeners.push_back(listener);
    }
    void removeWindowEventListener(IWindowEventListener* listener) {
        _windowEventListeners.erase(
            std::remove(_windowEventListeners.begin(), _windowEventListeners.end(), listener),
            _windowEventListeners.end()
        );
    }

    void addInputEventListener(IInputEventListener* listener) {
        _inputEventListeners.push_back(listener);
    }
    void removeInputEventListener(IInputEventListener* listener) {
        _inputEventListeners.erase(
            std::remove(_inputEventListeners.begin(), _inputEventListeners.end(), listener),
            _inputEventListeners.end()
        );
    }

protected:
    std::vector<IWindowEventListener*> _windowEventListeners;
    std::vector<IInputEventListener*>  _inputEventListeners;

    bool _VSyncEnabled = true;

    // Window size
    int _width = 800;
    int _height = 600;
};
