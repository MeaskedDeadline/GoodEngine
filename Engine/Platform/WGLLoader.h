#pragma once
#include "../Core/Core.h"

#if defined(GOOD_PLATFORM_WINDOWS)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/GL.h>

namespace Good
{

#ifndef WGL_CONTEXT_MAJOR_VERSION_ARB
#define WGL_CONTEXT_MAJOR_VERSION_ARB    0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB    0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB     0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#endif

    using PFN_wglCreateContextAttribsARB = HGLRC (WINAPI*)(HDC, HGLRC, const int*);
    using PFN_wglSwapIntervalEXT         = BOOL  (WINAPI*)(int);

    class WGLLoader
    {
    public:
        static bool LoadFromCurrentContext();
        static bool IsLoaded();

        static PFN_wglCreateContextAttribsARB GetCreateContextAttribs();
        static PFN_wglSwapIntervalEXT         GetSwapInterval();

    private:
        static PFN_wglCreateContextAttribsARB s_CreateContextAttribs;
        static PFN_wglSwapIntervalEXT         s_SwapInterval;
        static bool                           s_Loaded;
    };

} // namespace Good

#endif