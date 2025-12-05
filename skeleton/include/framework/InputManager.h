#pragma once

#include <glm/vec2.hpp>
#include <unordered_map>

#include "IInputEventListener.h"


// Input manager handling keyboard and mouse input events
// Is a state tracker
class InputManager : public IInputEventListener {
public:
    static InputManager& getInstance() {
        static InputManager s_instance;
        return s_instance;
    }

    // Reset input states at the beginning of each frame
    void beginFrame();

    // IInputEventListener interface implementation
    void onKeyEvent(KeyCode key, int scancode, KeyAction action, KeyMod mods) override;
    void onCharEvent(unsigned int codepoint) override;
    void onMouseButton(MouseButton button, KeyAction action, KeyMod mods) override;
    void onMouseMove(double x, double y) override;
    void onMouseScroll(double dx, double dy) override;

    // Query input states
    bool isKeyPressed(KeyCode key) const;
    bool isKeyPressedThisFrame(KeyCode key) const;
    bool isKeyReleasedThisFrame(KeyCode key) const;

    bool isMousePressed(MouseButton button) const;
    bool isMousePressedThisFrame(MouseButton button) const;
    bool isMouseReleasedThisFrame(MouseButton button) const;
    
    glm::vec2 getMousePosition() const;
    glm::vec2 getMouseDelta() const;

private:
    InputManager() = default;
    ~InputManager() = default;

    glm::vec2 _mousePos{0.0, 0.0};
    glm::vec2 _mouseDelta{0.0, 0.0};

    std::unordered_map<KeyCode, bool> _currentKeyState;
    std::unordered_map<KeyCode, bool> _previousKeyState;

    std::unordered_map<MouseButton, bool> _currentMouseState;
    std::unordered_map<MouseButton, bool> _previousMouseState;
};