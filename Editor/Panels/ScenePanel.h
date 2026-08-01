#pragma once
#include "Panel.h"
#include "Scene/SceneManager.h"

namespace Good
{

    class ScenePanel final : public Panel
    {
    public:
        ScenePanel();

        void OnRender() override;
        const char* GetTitle() const override { return "Scene Settings"; }

    private:
        std::string m_NewSceneName{"New Scene"};
    };

} // namespace Good