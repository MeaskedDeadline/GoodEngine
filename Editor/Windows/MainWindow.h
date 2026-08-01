#pragma once
#include "EditorWindow.h"
#include "Panels/HierarchyPanel.h"
#include "Panels/InspectorPanel.h"
#include "Panels/ContentBrowser.h"
#include "Panels/ConsolePanel.h"
#include "Panels/ViewportPanel.h"
#include "Panels/ScenePanel.h"
#include "Panels/ProfilerPanel.h"
#include "Layouts/DefaultLayout.h"
#include "Core/Engine.h"
#include <functional>

namespace Good
{

    class MainWindow final : public EditorWindow
    {
    public:
        MainWindow();
        ~MainWindow() override = default;

        bool Initialize(uint32 viewportW, uint32 viewportH);
        void Shutdown();

        void OnRender() override;
        void OnUpdate(float32 dt) override;
        const char* GetTitle() const override { return "GoodEngine Editor"; }

        void Resize(uint32 w, uint32 h);

        HierarchyPanel& GetHierarchy() { return m_Hierarchy; }
        InspectorPanel& GetInspector() { return m_Inspector; }
        ContentBrowser& GetContent()   { return m_Content;   }
        ConsolePanel&   GetConsole()   { return m_Console;   }
        ViewportPanel&  GetViewport()  { return m_Viewport;  }
        ProfilerPanel&  GetProfiler()  { return m_Profiler;  }

        std::function<void()> m_OnNewScene;
        std::function<void()> m_OnSaveScene;
        std::function<void()> m_OnLoadScene;

    private:
        void DrawTitleBar();
        void DrawMenuBar();
        void DrawToolbar();
        void LayoutPanels(uint32 w, uint32 h);

        HierarchyPanel m_Hierarchy;
        InspectorPanel m_Inspector;
        ContentBrowser m_Content;
        ConsolePanel   m_Console;
        ViewportPanel  m_Viewport;
        ScenePanel     m_ScenePanel;
        ProfilerPanel  m_Profiler;

        DefaultLayout  m_Layout;

        uint32 m_ViewportW{1280};
        uint32 m_ViewportH{720};

        bool m_ShowScenePanel{false};
        bool m_ShowProfiler{false};
        bool m_ShowFileMenu{false};

        bool    m_TitleDragging{false};
        float32 m_TitleDragX{0};
        float32 m_TitleDragY{0};
    };

} // namespace Good