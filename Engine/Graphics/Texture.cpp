#include "Texture.h"
#include "GLFunctions.h"
#include "../Core/Log.h"

#ifndef GL_RG
    #define GL_RG  0x8227
#endif
#ifndef GL_RED
    #define GL_RED 0x1903
#endif
#ifndef GL_RGB
    #define GL_RGB 0x1907
#endif

namespace Good
{

static GLenum ToFilter(TextureFilter f) { return f == TextureFilter::Nearest ? GL_NEAREST : GL_LINEAR; }
static GLenum ToWrap(TextureWrap w)
{
    switch (w)
    {
        case TextureWrap::ClampToEdge:    return GL_CLAMP_TO_EDGE;
        case TextureWrap::MirroredRepeat: return 0x8370;
        default:                          return GL_REPEAT;
    }
}

Texture::~Texture() { if (m_ID) glDeleteTextures(1, &m_ID); }

bool Texture::CreateFromData(const byte* pixels, uint32 w, uint32 h,
                              uint32 ch, const TextureConfig& cfg)
{
    m_Width = w; m_Height = h; m_Channels = ch;

    GLenum ifmt = GL_RGBA, fmt = GL_RGBA;
    switch (ch)
    {
        case 1: ifmt = GL_R8;    fmt = GL_RED; break;
        case 2: ifmt = GL_RG8;   fmt = GL_RG;  break;
        case 3: ifmt = GL_RGB;   fmt = GL_RGB; break;
        case 4: ifmt = GL_RGBA8; fmt = GL_RGBA; break;
    }

    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_2D, m_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(ToFilter(cfg.minFilter)));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(ToFilter(cfg.magFilter)));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     static_cast<GLint>(ToWrap(cfg.wrapS)));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     static_cast<GLint>(ToWrap(cfg.wrapT)));
    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(ifmt),
        static_cast<GLsizei>(w), static_cast<GLsizei>(h),
        0, fmt, GL_UNSIGNED_BYTE, pixels);
    if (cfg.generateMips && GL::GenerateMipmap) GL::GenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    GOOD_LOG_INFO("Texture", "Created {}x{} ch={} id={}", w, h, ch, m_ID);
    return true;
}

bool Texture::CreateWhite()
{
    byte p[4] = {255,255,255,255};
    return CreateFromData(p, 1, 1, 4, {});
}

bool Texture::CreateBlack()
{
    byte p[4] = {0,0,0,255};
    return CreateFromData(p, 1, 1, 4, {});
}

void Texture::Bind(uint32 slot) const
{
    if (GL::ActiveTexture) GL::ActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::Unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

} // namespace Good