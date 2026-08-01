#pragma once
#include "../Core/Core.h"
#include "Scene.h"

namespace Good
{

    class SceneManager : public NonCopyable
    {
    public:
        static SceneManager& Get();

        Ref<Scene> CreateScene(const std::string& name);
        Ref<Scene> LoadScene(const std::string& path);
        void       SetActiveScene(Ref<Scene> scene);
        Ref<Scene> GetActiveScene() const { return m_ActiveScene; }

        void Update(float32 dt);
        void Clear();

    private:
        SceneManager() = default;

        Ref<Scene>                        m_ActiveScene;
        std::vector<Ref<Scene>>           m_Scenes;
    };

} // namespace Good