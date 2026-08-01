#include "OpenGLContext.h"
#include "WGLLoader.h"
#include "../Core/Log.h"

#if defined(GOOD_PLATFORM_WINDOWS)

namespace Good
{

bool OpenGLContext::Create(void* windowHandle, void* deviceContext, bool vsync)
{
    (void)windowHandle;
    m_DC = static_cast<HDC>(deviceContext);

    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize        = sizeof(pfd);
    pfd.nVersion     = 1;
    pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType   = PFD_TYPE_RGBA;
    pfd.cColorBits   = 32;
    pfd.cDepthBits   = 24;
    pfd.cStencilBits = 8;
    pfd.cAlphaBits   = 8;
    pfd.iLayerType   = PFD_MAIN_PLANE;

    int pixelFormat = ::ChoosePixelFormat(m_DC, &pfd);
    if (!pixelFormat) { GOOD_LOG_ERROR("GLContext", "ChoosePixelFormat failed"); return false; }

    PIXELFORMATDESCRIPTOR chosen = {};
    DescribePixelFormat(m_DC, pixelFormat, sizeof(chosen), &chosen);

    if (chosen.dwFlags & PFD_GENERIC_FORMAT)
        GOOD_LOG_WARN("GLContext", "Software OpenGL detected (no GPU driver)");

    if (!SetPixelFormat(m_DC, pixelFormat, &chosen))
    {
        GOOD_LOG_ERROR("GLContext", "SetPixelFormat failed");
        return false;
    }

    if (!CreateLegacyContext(m_DC)) return false;

    const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    GOOD_LOG_INFO("GLContext", "Initial GL: {}", version ? version : "null");

    WGLLoader::LoadFromCurrentContext();

    if (WGLLoader::IsLoaded())
        CreateModernContext(m_DC);

    SetVSync(vsync);
    return true;
}

bool OpenGLContext::CreateModernContext(void* dc)
{
    auto createCtx = WGLLoader::GetCreateContextAttribs();
    if (!createCtx) return false;

    HDC   hdc    = static_cast<HDC>(dc);
    HGLRC legacy = m_Context;

    const int versions[][2] = { {4,6},{4,5},{4,3},{4,0},{3,3} };

    for (auto& ver : versions)
    {
        const int attribs[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB, ver[0],
            WGL_CONTEXT_MINOR_VERSION_ARB, ver[1],
            WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0
        };

        HGLRC modern = createCtx(hdc, nullptr, attribs);
        if (!modern) continue;

        wglMakeCurrent(nullptr, nullptr);
        if (wglMakeCurrent(hdc, modern))
        {
            if (legacy) wglDeleteContext(legacy);
            m_Context      = modern;
            m_MajorVersion = ver[0];
            m_MinorVersion = ver[1];
            m_Modern       = true;
            GOOD_LOG_INFO("GLContext", "OpenGL {}.{} Core created", ver[0], ver[1]);
            return true;
        }

        wglDeleteContext(modern);
        if (legacy) wglMakeCurrent(hdc, legacy);
    }

    return false;
}

bool OpenGLContext::CreateLegacyContext(void* dc)
{
    HDC hdc = static_cast<HDC>(dc);
    HGLRC ctx = wglCreateContext(hdc);
    if (!ctx) return false;
    if (!wglMakeCurrent(hdc, ctx)) { wglDeleteContext(ctx); return false; }
    m_Context = ctx;
    m_MajorVersion = 1;
    m_MinorVersion = 1;
    m_Modern = false;
    GOOD_LOG_TRACE("GLContext", "Creating temporary legacy context...");
    return true;
}

void OpenGLContext::Destroy()
{
    if (m_Context)
    {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(m_Context);
        m_Context = nullptr;
    }
    m_DC = nullptr;
}

void OpenGLContext::SwapBuffers()
{
    if (m_DC) ::SwapBuffers(m_DC);
}

void OpenGLContext::SetVSync(bool enabled)
{
    auto fn = WGLLoader::GetSwapInterval();
    if (fn) { fn(enabled ? 1 : 0); GOOD_LOG_INFO("GLContext", "VSync: {}", enabled ? "on" : "off"); }
    else      GOOD_LOG_WARN("GLContext", "VSync not available");
}

} // namespace Good

#endif