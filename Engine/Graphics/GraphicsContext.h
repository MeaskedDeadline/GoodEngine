#pragma once
#include "../Core/Core.h"

namespace Good
{

    class GraphicsContext : public NonCopyable
    {
    public:
        static GraphicsContext& Get();

        bool Initialize();
        void Shutdown();
        bool IsInitialized() const { return m_Initialized; }

    private:
        GraphicsContext() = default;
        bool LoadOpenGLFunctions();
        bool m_Initialized{false};
    };

} // namespace Good