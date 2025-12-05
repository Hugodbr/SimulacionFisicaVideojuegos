#pragma once


// Interface for window event listeners
// Classes implementing this interface can receive window events
class IWindowEventListener {
public:
    virtual ~IWindowEventListener() = default;

    virtual void onDisplay() {}
    virtual void onResize(int width, int height) {}
    virtual void onClose() {}
    virtual void onFocusChanged(bool focused) {}
};