#pragma once
#include "../Core/Core.h"

#if defined(GOOD_PLATFORM_WINDOWS)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <gl/GL.h>
#endif

namespace Good
{

    class OpenGLContext
    {
    public:
        OpenGLContext()  = default;
        ~OpenGLContext() = default;

        bool Create(void* windowHandle, void* deviceContext, bool vsync);
        void Destroy();
        void SwapBuffers();
        void SetVSync(bool enabled);

        int  GetMajorVersion() const { return m_MajorVersion; }
        int  GetMinorVersion() const { return m_MinorVersion; }
        bool IsModern()        const { return m_Modern;       }

    private:
        bool CreateModernContext(void* dc);
        bool CreateLegacyContext(void* dc);

#if defined(GOOD_PLATFORM_WINDOWS)
        HDC   m_DC{nullptr};
        HGLRC m_Context{nullptr};
#endif

        int  m_MajorVersion{0};
        int  m_MinorVersion{0};
        bool m_Modern{false};
    };

} // namespace Good