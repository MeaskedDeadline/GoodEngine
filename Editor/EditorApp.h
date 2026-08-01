#pragma once
#include "../Engine/Core/App.h"
#include "../Engine/Core/Engine.h"
#include "EditorLayer.h"

namespace Good
{

    class EditorApp final : public App
    {
    public:
        EditorApp()  = default;
        ~EditorApp() = default;

        void OnStart()  override;
        void OnUpdate() override;
        void OnRender() override;
        void OnUI()     override;
        void OnStop()   override;

        void OnFixedUpdate() override;

        const char* GetName() const override { return "GoodEngine Editor"; }

    private:
        EditorLayer m_EditorLayer;
    };

} // namespace Good