#pragma once

#include "InputKeys.h"


// Interface for input event listeners (optional event handlers)
// Classes implementing this interface can receive input events
// API-agnostic
class IInputEventListener {
public:
    virtual ~IInputEventListener() = default;

    virtual void onKeyEvent(KeyCode key, int scancode, KeyAction action, KeyMod mods) {}
    virtual void onCharEvent(unsigned int codepoint) {}
    virtual void onMouseButton(MouseButton button, KeyAction action, KeyMod mods) {}
    virtual void onMouseMove(double x, double y) {}
    virtual void onMouseScroll(double dx, double dy) {}
};
