#pragma once
#include "../Core/Core.h"
#include "../Graphics/Framebuffer.h"

namespace Good
{

    class RenderPass
    {
    public:
        RenderPass()  = default;
        ~RenderPass() = default;

        void SetName(const std::string& name) { m_Name = name; }
        const std::string& GetName() const    { return m_Name; }

        void SetFramebuffer(Ref<Framebuffer> fb) { m_Framebuffer = std::move(fb); }
        Ref<Framebuffer> GetFramebuffer() const  { return m_Framebuffer; }

        void Begin();
        void End();

        bool IsEnabled() const           { return m_Enabled; }
        void SetEnabled(bool e)          { m_Enabled = e; }

    private:
        std::string      m_Name{"RenderPass"};
        Ref<Framebuffer> m_Framebuffer;
        bool             m_Enabled{true};
    };

} // namespace Good