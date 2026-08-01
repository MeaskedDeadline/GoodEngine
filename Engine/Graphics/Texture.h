#pragma once
#include "../Core/Core.h"

namespace Good
{

    enum class TextureFilter : uint8 { Nearest, Linear };
    enum class TextureWrap   : uint8 { Repeat, ClampToEdge, MirroredRepeat };

    struct TextureConfig
    {
        TextureFilter minFilter{TextureFilter::Linear};
        TextureFilter magFilter{TextureFilter::Linear};
        TextureWrap   wrapS{TextureWrap::Repeat};
        TextureWrap   wrapT{TextureWrap::Repeat};
        bool          generateMips{true};
    };

    class Texture
    {
    public:
        Texture()  = default;
        ~Texture();

        bool CreateFromData(const byte* pixels, uint32 width, uint32 height,
                            uint32 channels, const TextureConfig& config = {});
        bool CreateWhite();
        bool CreateBlack();

        void Bind(uint32 slot = 0) const;
        void Unbind() const;

        uint32 GetID()       const { return m_ID;       }
        uint32 GetWidth()    const { return m_Width;    }
        uint32 GetHeight()   const { return m_Height;   }
        uint32 GetChannels() const { return m_Channels; }

    private:
        uint32 m_ID{0};
        uint32 m_Width{0};
        uint32 m_Height{0};
        uint32 m_Channels{0};
    };

} // namespace Good