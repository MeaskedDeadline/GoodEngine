#include "GraphicsContext.h"
#include "GLFunctions.h"
#include "../Core/Log.h"

namespace Good
{

    GraphicsContext& GraphicsContext::Get()
    {
        static GraphicsContext s_Instance;
        return s_Instance;
    }

    bool GraphicsContext::Initialize()
    {
        if (m_Initialized) return true;

        if (!GL::LoadFunctions())
        {
            GOOD_LOG_WARN("GraphicsContext", "Modern GL functions not loaded (legacy mode)");
        }

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        m_Initialized = true;
        GOOD_LOG_INFO("GraphicsContext", "Initialized");
        return true;
    }

    void GraphicsContext::Shutdown()
    {
        m_Initialized = false;
        GOOD_LOG_INFO("GraphicsContext", "Shutdown");
    }

} // namespace Good