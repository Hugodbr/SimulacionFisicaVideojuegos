#pragma once


// OpenGL implementation of the RenderAPI interface
class GLRenderEngine
{
public:
    GLRenderEngine();
    virtual ~GLRenderEngine();
    // Initialization
    virtual bool initialize(void* windowHandle);
    virtual void shutdown();

    // Per-frame operations
    virtual void beginFrame();
    virtual void endFrame();
    virtual void present();

    // Clear framebuffer
    virtual void clear(float r, float g, float b, float a);
    // Create resources

    // VSync / presentation
    virtual void setVSync(bool enabled);

protected:
    void* _windowHandle;
    bool  _initialized;
};