#pragma once
#include "Component.h"
#include "../Rendering/Sprite.h"

namespace Good
{

    class SpriteComponent final : public Component
    {
    public:
        const char* GetTypeName() const override { return "SpriteComponent"; }

        Sprite&       GetSprite()       { return m_Sprite; }
        const Sprite& GetSprite() const { return m_Sprite; }

        void SetLayer(int32 layer)  { m_Layer = layer; }
        int32 GetLayer()      const { return m_Layer;  }

    private:
        Sprite m_Sprite;
        int32  m_Layer{0};
    };

} // namespace Good