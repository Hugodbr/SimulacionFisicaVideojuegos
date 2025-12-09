#include "InputManager.h"

#include <iostream>


void InputManager::beginFrame()
{
    _mouseDelta = glm::vec2{0.0f, 0.0f};

    _previousKeyState = _currentKeyState;
    _previousMouseState = _currentMouseState;

    // _currentKeyState = {};
    // _currentMouseState = {};
}


//=========================================================================================================
// IInputEventListener interface implementation
//=========================================================================================================
void InputManager::onKeyEvent(KeyCode key, int scancode, KeyAction action, KeyMod mods)
{
    if (action == KeyAction::Press) {
        _currentKeyState[key] = true;
    } else if (action == KeyAction::Release) {
        _currentKeyState[key] = false;
    }
}

void InputManager::onCharEvent(unsigned int codepoint)
{
    // Handle character input if needed
}

void InputManager::onMouseButton(MouseButton button, KeyAction action, KeyMod mods)
{
    // std::cout << "Mouse button event: " << static_cast<int>(button) << " Action: " << static_cast<int>(action) << std::endl;

    if (action == KeyAction::Press) {
        _currentMouseState[button] = true;

    } else if (action == KeyAction::Release) {
        _currentMouseState[button] = false;
    }
}

void InputManager::onMouseMove(double x, double y)
{
    glm::vec2 newMousePos{static_cast<float>(x), static_cast<float>(y)};
    _mouseDelta = newMousePos - _mousePos;
    _mousePos = newMousePos;

    // std::cout << "Mouse moved to: (" << x << ", " << y << ")\n";
}

void InputManager::onMouseScroll(double dx, double dy)
{
    // Handle mouse scroll if needed
}


//=========================================================================================================
// Query input states
//=========================================================================================================
bool InputManager::isKeyPressed(KeyCode key) const
{
    auto it = _currentKeyState.find(key);
    return it != _currentKeyState.end() && it->second;
}

bool InputManager::isKeyPressedThisFrame(KeyCode key) const
{
    bool currentlyPressed = false;
    auto itCurrent = _currentKeyState.find(key);
    if (itCurrent != _currentKeyState.end()) {
        currentlyPressed = itCurrent->second;
    }

    bool previouslyPressed = false;
    auto itPrevious = _previousKeyState.find(key);
    if (itPrevious != _previousKeyState.end()) {
        previouslyPressed = itPrevious->second;
    }

    return currentlyPressed && !previouslyPressed;
}


bool InputManager::isKeyReleasedThisFrame(KeyCode key) const
{
    bool currentlyPressed = false;
    auto itCurrent = _currentKeyState.find(key);
    if (itCurrent != _currentKeyState.end()) {
        currentlyPressed = itCurrent->second;
    }

    bool previouslyPressed = false;
    auto itPrevious = _previousKeyState.find(key);
    if (itPrevious != _previousKeyState.end()) {
        previouslyPressed = itPrevious->second;
    }

    return !currentlyPressed && previouslyPressed;
}

bool InputManager::isMousePressed(MouseButton button) const
{
    auto it = _currentMouseState.find(button);
    return it != _currentMouseState.end() && it->second;
}

bool InputManager::isMousePressedThisFrame(MouseButton button) const
{
    // std::cout << "Checking if mouse button " << static_cast<int>(button) << " was pressed this frame.\n";

    bool currentlyPressed = false;
    auto itCurrent = _currentMouseState.find(button);
    if (itCurrent != _currentMouseState.end()) {
        currentlyPressed = itCurrent->second;
    }

    bool previouslyPressed = false;
    auto itPrevious = _previousMouseState.find(button);
    if (itPrevious != _previousMouseState.end()) {
        previouslyPressed = itPrevious->second;
    }

    return currentlyPressed && !previouslyPressed;
}

bool InputManager::isMouseReleasedThisFrame(MouseButton button) const
{
    bool currentlyPressed = false;
    auto itCurrent = _currentMouseState.find(button);
    if (itCurrent != _currentMouseState.end()) {
        currentlyPressed = itCurrent->second;
    }

    bool previouslyPressed = false;
    auto itPrevious = _previousMouseState.find(button);
    if (itPrevious != _previousMouseState.end()) {
        previouslyPressed = itPrevious->second;
    }

    return !currentlyPressed && previouslyPressed;
}

glm::vec2 InputManager::getMousePosition() const
{
    return _mousePos;
}

glm::vec2 InputManager::getMouseDelta() const
{
    return _mouseDelta;
}
