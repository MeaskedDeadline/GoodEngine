#pragma once
#include "Core/Core.h"
#include "Scene/SceneManager.h"
#include "Windows/MainWindow.h"

namespace Good
{

    class EditorLayer
    {
    public:
        EditorLayer();
        ~EditorLayer();

        bool Initialize(uint32 viewportW, uint32 viewportH);
        void Shutdown();
        void Update(float32 dt);
        void Render();
        void Resize(uint32 w, uint32 h);

        MainWindow& GetMainWindow() { return m_MainWindow; }

    private:
        void SetupDefaultScene();
        void SetupDebugConsole();

        MainWindow m_MainWindow;
        Ref<Scene> m_ActiveScene;
    };

} // namespace Good