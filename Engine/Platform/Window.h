#pragma once
#include "../Core/Core.h"
#include "WindowEvent.h"
#include "OpenGLContext.h"
#include <functional>
#include "UI/UIInput.h"

namespace Good
{

    using EventCallback = std::function<void(const WindowEvent&)>;

    struct WindowConfig
    {
        std::string title      = "GoodEngine";
        uint32      width      = 1280;
        uint32      height     = 720;
        bool        fullscreen = false;
        bool        vsync      = true;
        bool        resizable  = true;
    };

    class Window : public NonCopyable
    {
    public:
        static Window& Get();

        bool Initialize(const WindowConfig& config);
        void Shutdown();
        void PollEvents();
        void BeginFrame();
        void EndFrame();

        bool    IsOpen()      const { return m_Open;   }
        uint32  Width()       const { return m_Width;  }
        uint32  Height()      const { return m_Height; }
        float32 AspectRatio() const
        {
            return m_Height > 0
                ? static_cast<float32>(m_Width) / static_cast<float32>(m_Height)
                : 1.0f;
        }

        void SetTitle(const std::string& title);
        void SetVSync(bool enabled);
        void SetEventCallback(EventCallback cb) { m_EventCallback = std::move(cb); }
        void* GetNativeHandle() const { return m_NativeHandle; }

        bool          m_Open{false};
        uint32        m_Width{0};
        uint32        m_Height{0};
        EventCallback m_EventCallback;

    private:
        Window() = default;

        WindowConfig  m_Config;
        void*         m_NativeHandle{nullptr};
        OpenGLContext m_GLContext;

#if defined(GOOD_PLATFORM_WINDOWS)
        void* m_HInstance{nullptr};
        void* m_HDC{nullptr};
#endif
    };

} // namespace Good