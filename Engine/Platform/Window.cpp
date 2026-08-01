#include "Window.h"
#include "Core/Log.h"
#include "Input/Input.h"
#include "UI/UIInput.h"

#if defined(GOOD_PLATFORM_WINDOWS)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <gl/GL.h>

namespace Good
{

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

Window& Window::Get()
{
    static Window s_Instance;
    return s_Instance;
}

bool Window::Initialize(const WindowConfig& config)
{
    m_Config    = config;
    m_Width     = config.width;
    m_Height    = config.height;
    m_HInstance = GetModuleHandleA(nullptr);

    WNDCLASSEXA wc   = {};
    wc.cbSize        = sizeof(WNDCLASSEXA);
    wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = static_cast<HINSTANCE>(m_HInstance);
    wc.hCursor       = LoadCursorA(nullptr, IDC_ARROW);
    wc.lpszClassName = "GoodEngineWnd";
    wc.hIcon         = LoadIconA(nullptr, IDI_APPLICATION);

    if (!RegisterClassExA(&wc))
    {
        GOOD_LOG_ERROR("Window", "RegisterClassEx failed");
        return false;
    }

    DWORD style = WS_POPUP | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX |
              WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    if (!config.resizable) style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);

    RECT r = { 0, 0,
               static_cast<LONG>(config.width),
               static_cast<LONG>(config.height) };
    AdjustWindowRect(&r, style, FALSE);

    HWND hwnd = CreateWindowExA(
        0, "GoodEngineWnd", config.title.c_str(), style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        r.right - r.left, r.bottom - r.top,
        nullptr, nullptr,
        static_cast<HINSTANCE>(m_HInstance), this);

    if (!hwnd)
    {
        GOOD_LOG_ERROR("Window", "CreateWindowEx failed");
        return false;
    }

    m_NativeHandle = hwnd;
    m_HDC          = GetDC(hwnd);

    if (!m_GLContext.Create(hwnd, m_HDC, config.vsync))
    {
        GOOD_LOG_FATAL("Window", "OpenGL context failed!");
        return false;
    }

    ShowWindow(hwnd, SW_MAXIMIZE);
    UpdateWindow(hwnd);

    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    m_Width  = static_cast<uint32>(clientRect.right - clientRect.left);
    m_Height = static_cast<uint32>(clientRect.bottom - clientRect.top);

    m_Open = true;

    GOOD_LOG_INFO("Window", "'{}' {}x{} ready (GL {}.{})",
        config.title, m_Width, m_Height,
        m_GLContext.GetMajorVersion(),
        m_GLContext.GetMinorVersion());

    return true;
}

void Window::Shutdown()
{
    m_GLContext.Destroy();
    if (m_HDC && m_NativeHandle)
        ReleaseDC(static_cast<HWND>(m_NativeHandle), static_cast<HDC>(m_HDC));
    if (m_NativeHandle)
        DestroyWindow(static_cast<HWND>(m_NativeHandle));
    UnregisterClassA("GoodEngineWnd", static_cast<HINSTANCE>(m_HInstance));
    m_Open = false;
}

void Window::PollEvents()
{
    MSG msg;
    while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

void Window::BeginFrame()
{
    glDisable(GL_SCISSOR_TEST);
    glViewport(0, 0,
        static_cast<GLsizei>(m_Width),
        static_cast<GLsizei>(m_Height));
    glClearColor(0.12f, 0.12f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::EndFrame() { m_GLContext.SwapBuffers(); }

void Window::SetTitle(const std::string& t)
{
    if (m_NativeHandle) SetWindowTextA(static_cast<HWND>(m_NativeHandle), t.c_str());
}

void Window::SetVSync(bool e)
{
    m_GLContext.SetVSync(e);
    m_Config.vsync = e;
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Window* win = nullptr;
    if (msg == WM_NCCREATE)
    {
        auto* cs = reinterpret_cast<CREATESTRUCTA*>(lParam);
        win = static_cast<Window*>(cs->lpCreateParams);
        SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(win));
    }
    else
    {
        win = reinterpret_cast<Window*>(GetWindowLongPtrA(hwnd, GWLP_USERDATA));
    }

    if (!win) return DefWindowProcA(hwnd, msg, wParam, lParam);

    switch (msg)
    {
        case WM_CLOSE:
        {
            win->m_Open = false;
            WindowEvent e{}; e.type = WindowEventType::Close;
            if (win->m_EventCallback) win->m_EventCallback(e);
            return 0;
        }

        case WM_SIZE:
        {
            win->m_Width  = static_cast<uint32>(LOWORD(lParam));
            win->m_Height = static_cast<uint32>(HIWORD(lParam));
            WindowEvent e{}; e.type = WindowEventType::Resize;
            e.resize.width  = static_cast<int32>(win->m_Width);
            e.resize.height = static_cast<int32>(win->m_Height);
            if (win->m_EventCallback) win->m_EventCallback(e);
            return 0;
        }

        case WM_SETFOCUS:  { WindowEvent e{}; e.type = WindowEventType::Focus;     if (win->m_EventCallback) win->m_EventCallback(e); return 0; }
        case WM_KILLFOCUS: { WindowEvent e{}; e.type = WindowEventType::LostFocus; if (win->m_EventCallback) win->m_EventCallback(e); return 0; }

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
            int32 keyCode = static_cast<int32>(wParam);
            WindowEvent e{}; e.type = WindowEventType::KeyPressed;
            e.key.keyCode = keyCode;
            e.key.repeat  = (lParam & 0x40000000) != 0;
            if (win->m_EventCallback) win->m_EventCallback(e);

            Input::Get().OnKeyEvent(keyCode, true);
            UIInput::Get().OnKey(keyCode, true);
            return 0;
        }

        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            int32 keyCode = static_cast<int32>(wParam);
            WindowEvent e{}; e.type = WindowEventType::KeyReleased;
            e.key.keyCode = keyCode;
            e.key.repeat  = false;
            if (win->m_EventCallback) win->m_EventCallback(e);

            Input::Get().OnKeyEvent(keyCode, false);
            UIInput::Get().OnKey(keyCode, false);
            return 0;
        }

        case WM_CHAR:
        {
            uint32 cp = static_cast<uint32>(wParam);
            WindowEvent e{}; e.type = WindowEventType::CharTyped;
            e.charTyped.codepoint = cp;
            if (win->m_EventCallback) win->m_EventCallback(e);
            UIInput::Get().OnChar(cp);
            return 0;
        }

        case WM_LBUTTONDOWN:
        {
            WindowEvent e{}; e.type = WindowEventType::MouseButtonPressed; e.mouse.button = 0;
            if (win->m_EventCallback) win->m_EventCallback(e);
            Input::Get().OnMouseButtonEvent(0, true);
            return 0;
        }
        case WM_RBUTTONDOWN:
        {
            WindowEvent e{}; e.type = WindowEventType::MouseButtonPressed; e.mouse.button = 1;
            if (win->m_EventCallback) win->m_EventCallback(e);
            Input::Get().OnMouseButtonEvent(1, true);
            return 0;
        }
        case WM_MBUTTONDOWN:
        {
            WindowEvent e{}; e.type = WindowEventType::MouseButtonPressed; e.mouse.button = 2;
            if (win->m_EventCallback) win->m_EventCallback(e);
            Input::Get().OnMouseButtonEvent(2, true);
            return 0;
        }
        case WM_LBUTTONUP:
        {
            WindowEvent e{}; e.type = WindowEventType::MouseButtonReleased; e.mouse.button = 0;
            if (win->m_EventCallback) win->m_EventCallback(e);
            Input::Get().OnMouseButtonEvent(0, false);
            return 0;
        }
        case WM_RBUTTONUP:
        {
            WindowEvent e{}; e.type = WindowEventType::MouseButtonReleased; e.mouse.button = 1;
            if (win->m_EventCallback) win->m_EventCallback(e);
            Input::Get().OnMouseButtonEvent(1, false);
            return 0;
        }
        case WM_MBUTTONUP:
        {
            WindowEvent e{}; e.type = WindowEventType::MouseButtonReleased; e.mouse.button = 2;
            if (win->m_EventCallback) win->m_EventCallback(e);
            Input::Get().OnMouseButtonEvent(2, false);
            return 0;
        }
        case WM_MOUSEMOVE:
        {
            float32 x = static_cast<float32>(GET_X_LPARAM(lParam));
            float32 y = static_cast<float32>(GET_Y_LPARAM(lParam));
            WindowEvent e{}; e.type = WindowEventType::MouseMoved;
            e.mousePos.x = x; e.mousePos.y = y;
            if (win->m_EventCallback) win->m_EventCallback(e);
            Input::Get().OnMouseMove(x, y);
            return 0;
        }
    case WM_MOUSEWHEEL:
            {
                float32 delta = static_cast<float32>(GET_WHEEL_DELTA_WPARAM(wParam))
                              / static_cast<float32>(WHEEL_DELTA);
                WindowEvent e{}; e.type = WindowEventType::MouseScrolled;
                e.scroll.xOff = 0.0f; e.scroll.yOff = delta;
                if (win->m_EventCallback) win->m_EventCallback(e);
                Input::Get().OnMouseScroll(0.0f, delta);
                UIInput::Get().OnMouseScroll(0.0f, delta);
                return 0;
            }
        case WM_DESTROY: PostQuitMessage(0); return 0;
        default: break;
    }

    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

} // namespace Good

#elif defined(GOOD_PLATFORM_LINUX)

namespace Good
{
    Window& Window::Get() { static Window s_Instance; return s_Instance; }
    bool Window::Initialize(const WindowConfig&) { return false; }
    void Window::Shutdown()   {}
    void Window::PollEvents() {}
    void Window::BeginFrame() {}
    void Window::EndFrame()   {}
    void Window::SetTitle(const std::string&) {}
    void Window::SetVSync(bool) {}
}

#endif