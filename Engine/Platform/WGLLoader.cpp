#include "WGLLoader.h"

#if defined(GOOD_PLATFORM_WINDOWS)

#include "../Core/Log.h"

namespace Good
{

    PFN_wglCreateContextAttribsARB WGLLoader::s_CreateContextAttribs = nullptr;
    PFN_wglSwapIntervalEXT         WGLLoader::s_SwapInterval         = nullptr;
    bool                           WGLLoader::s_Loaded               = false;

    template<typename T>
    static void LoadFunc(T& fn, const char* name)
    {
        PROC p = wglGetProcAddress(name);
        if (!p || p == reinterpret_cast<PROC>(1) || p == reinterpret_cast<PROC>(2)
               || p == reinterpret_cast<PROC>(3) || p == reinterpret_cast<PROC>(-1))
        {
            HMODULE gl = GetModuleHandleA("opengl32.dll");
            if (gl) p = reinterpret_cast<PROC>(GetProcAddress(gl, name));
        }
        if (p) { union { PROC src; T dst; } u; u.src = p; fn = u.dst; }
    }

    bool WGLLoader::LoadFromCurrentContext()
    {
        LoadFunc(s_CreateContextAttribs, "wglCreateContextAttribsARB");
        LoadFunc(s_SwapInterval,         "wglSwapIntervalEXT");
        s_Loaded = (s_CreateContextAttribs != nullptr);
        GOOD_LOG_INFO("WGLLoader", "CreateContextAttribs: {}", s_CreateContextAttribs ? "OK" : "missing");
        GOOD_LOG_INFO("WGLLoader", "SwapInterval:         {}", s_SwapInterval ? "OK" : "missing");
        return s_Loaded;
    }

    bool                           WGLLoader::IsLoaded()               { return s_Loaded;               }
    PFN_wglCreateContextAttribsARB WGLLoader::GetCreateContextAttribs(){ return s_CreateContextAttribs; }
    PFN_wglSwapIntervalEXT         WGLLoader::GetSwapInterval()        { return s_SwapInterval;         }

} // namespace Good

#endif