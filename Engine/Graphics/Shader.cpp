#include "Shader.h"
#include "GLFunctions.h"
#include "../Core/Log.h"
#include <fstream>
#include <sstream>

namespace Good
{

Shader::Shader()  = default;
Shader::~Shader() { if (m_ID) GL::DeleteProgram(m_ID); }

bool Shader::LoadFromSource(const std::string& vs, const std::string& fs)
{
    uint32 v = Compile(GL_VERTEX_SHADER,   vs);
    uint32 f = Compile(GL_FRAGMENT_SHADER, fs);

    if (!v || !f)
    {
        if (v) GL::DeleteShader(v);
        if (f) GL::DeleteShader(f);
        return false;
    }

    m_ID = GL::CreateProgram();
    GL::AttachShader(m_ID, v);
    GL::AttachShader(m_ID, f);
    GL::LinkProgram(m_ID);

    GLint ok = 0;
    GL::GetProgramiv(m_ID, GL_LINK_STATUS, &ok);
    if (!ok)
    {
        char log[1024];
        GL::GetProgramInfoLog(m_ID, sizeof(log), nullptr, log);
        GOOD_LOG_ERROR("Shader", "Link failed: {}", log);
        GL::DeleteProgram(m_ID);
        m_ID = 0;
    }

    GL::DeleteShader(v);
    GL::DeleteShader(f);
    return m_ID != 0;
}

bool Shader::LoadFromFiles(const std::string& vp, const std::string& fp)
{
    auto read = [](const std::string& path) -> std::string
    {
        std::ifstream f(path);
        if (!f.is_open()) return "";
        std::stringstream ss;
        ss << f.rdbuf();
        return ss.str();
    };

    std::string vs = read(vp);
    std::string fs = read(fp);

    if (vs.empty()) { GOOD_LOG_ERROR("Shader", "Cannot read: {}", vp); return false; }
    if (fs.empty()) { GOOD_LOG_ERROR("Shader", "Cannot read: {}", fp); return false; }

    return LoadFromSource(vs, fs);
}

uint32 Shader::Compile(uint32 type, const std::string& src)
{
    GLuint shader = GL::CreateShader(static_cast<GLenum>(type));
    const char* s = src.c_str();
    GL::ShaderSource(shader, 1, &s, nullptr);
    GL::CompileShader(shader);

    GLint ok = 0;
    GL::GetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok)
    {
        char log[1024];
        GL::GetShaderInfoLog(shader, sizeof(log), nullptr, log);
        GOOD_LOG_ERROR("Shader", "Compile failed ({}): {}",
            type == GL_VERTEX_SHADER ? "VERT" : "FRAG", log);
        GL::DeleteShader(shader);
        return 0;
    }
    return shader;
}

void Shader::Bind()   const { if (GL::UseProgram) GL::UseProgram(m_ID); }
void Shader::Unbind() const { if (GL::UseProgram) GL::UseProgram(0); }

int32 Shader::GetLocation(const std::string& name)
{
    auto it = m_Cache.find(name);
    if (it != m_Cache.end()) return it->second;
    int32 loc = GL::GetUniformLocation ? GL::GetUniformLocation(m_ID, name.c_str()) : -1;
    m_Cache[name] = loc;
    return loc;
}

void Shader::SetInt(const std::string& n, int32 v)       { if (GL::Uniform1i)        GL::Uniform1i(GetLocation(n), v); }
void Shader::SetFloat(const std::string& n, float32 v)   { if (GL::Uniform1f)        GL::Uniform1f(GetLocation(n), v); }
void Shader::SetVec2(const std::string& n, const Vec2& v){ if (GL::Uniform2f)        GL::Uniform2f(GetLocation(n), v.x, v.y); }
void Shader::SetVec3(const std::string& n, const Vec3& v){ if (GL::Uniform3f)        GL::Uniform3f(GetLocation(n), v.x, v.y, v.z); }
void Shader::SetVec4(const std::string& n, const Vec4& v){ if (GL::Uniform4f)        GL::Uniform4f(GetLocation(n), v.x, v.y, v.z, v.w); }
void Shader::SetColor(const std::string& n, const Color& c){ if (GL::Uniform4f)      GL::Uniform4f(GetLocation(n), c.r, c.g, c.b, c.a); }
void Shader::SetMat4(const std::string& n, const Mat4& m){ if (GL::UniformMatrix4fv) GL::UniformMatrix4fv(GetLocation(n), 1, GL_FALSE, m.Data()); }

} // namespace Good