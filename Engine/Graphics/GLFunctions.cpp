#include "GLFunctions.h"
#include "../Core/Log.h"

#if defined(GOOD_PLATFORM_WINDOWS)
    #include <windows.h>
#elif defined(GOOD_PLATFORM_LINUX)
    #include <GL/glx.h>
#endif

namespace Good { namespace GL
{

template<typename T>
static void Load(T& fn, const char* name)
{
#if defined(GOOD_PLATFORM_WINDOWS)
    PROC p = wglGetProcAddress(name);
    if (!p || p == reinterpret_cast<PROC>(1) || p == reinterpret_cast<PROC>(2)
           || p == reinterpret_cast<PROC>(3) || p == reinterpret_cast<PROC>(-1))
    {
        HMODULE gl = GetModuleHandleA("opengl32.dll");
        if (gl) p = reinterpret_cast<PROC>(GetProcAddress(gl, name));
    }
    if (p) { union { PROC src; T dst; } u; u.src = p; fn = u.dst; }
#elif defined(GOOD_PLATFORM_LINUX)
    void* p = reinterpret_cast<void*>(glXGetProcAddress(reinterpret_cast<const GLubyte*>(name)));
    if (p)  { union { void* src; T dst; } u; u.src = p; fn = u.dst; }
#endif
}

void  (*ShaderSource)(GLuint,GLsizei,const GLchar**,const GLint*)   = nullptr;
GLuint(*CreateShader)(GLenum)                                       = nullptr;
void  (*CompileShader)(GLuint)                                      = nullptr;
void  (*GetShaderiv)(GLuint,GLenum,GLint*)                          = nullptr;
void  (*GetShaderInfoLog)(GLuint,GLsizei,GLsizei*,GLchar*)          = nullptr;
void  (*DeleteShader)(GLuint)                                       = nullptr;
GLuint(*CreateProgram)()                                            = nullptr;
void  (*AttachShader)(GLuint,GLuint)                                = nullptr;
void  (*LinkProgram)(GLuint)                                        = nullptr;
void  (*GetProgramiv)(GLuint,GLenum,GLint*)                         = nullptr;
void  (*GetProgramInfoLog)(GLuint,GLsizei,GLsizei*,GLchar*)         = nullptr;
void  (*UseProgram)(GLuint)                                         = nullptr;
void  (*DeleteProgram)(GLuint)                                      = nullptr;
GLint (*GetUniformLocation)(GLuint,const GLchar*)                   = nullptr;
void  (*Uniform1i)(GLint,GLint)                                     = nullptr;
void  (*Uniform1f)(GLint,GLfloat)                                   = nullptr;
void  (*Uniform2f)(GLint,GLfloat,GLfloat)                           = nullptr;
void  (*Uniform3f)(GLint,GLfloat,GLfloat,GLfloat)                   = nullptr;
void  (*Uniform4f)(GLint,GLfloat,GLfloat,GLfloat,GLfloat)           = nullptr;
void  (*UniformMatrix4fv)(GLint,GLsizei,GLboolean,const GLfloat*)   = nullptr;
void  (*GenBuffers)(GLsizei,GLuint*)                                = nullptr;
void  (*DeleteBuffers)(GLsizei,const GLuint*)                       = nullptr;
void  (*BindBuffer)(GLenum,GLuint)                                  = nullptr;
void  (*BufferData)(GLenum,GLsizeiptr,const void*,GLenum)           = nullptr;
void  (*BufferSubData)(GLenum,GLintptr,GLsizeiptr,const void*)      = nullptr;
void  (*GenVertexArrays)(GLsizei,GLuint*)                           = nullptr;
void  (*DeleteVertexArrays)(GLsizei,const GLuint*)                  = nullptr;
void  (*BindVertexArray)(GLuint)                                    = nullptr;
void  (*EnableVertexAttribArray)(GLuint)                            = nullptr;
void  (*VertexAttribPointer)(GLuint,GLint,GLenum,GLboolean,GLsizei,const void*) = nullptr;
void  (*ActiveTexture)(GLenum)                                      = nullptr;
void  (*GenerateMipmap)(GLenum)                                     = nullptr;
void  (*GenFramebuffers)(GLsizei,GLuint*)                           = nullptr;
void  (*DeleteFramebuffers)(GLsizei,const GLuint*)                  = nullptr;
void  (*BindFramebuffer)(GLenum,GLuint)                             = nullptr;
void  (*FramebufferTexture2D)(GLenum,GLenum,GLenum,GLuint,GLint)    = nullptr;
GLenum(*CheckFramebufferStatus)(GLenum)                             = nullptr;
void  (*GenRenderbuffers)(GLsizei,GLuint*)                          = nullptr;
void  (*DeleteRenderbuffers)(GLsizei,const GLuint*)                 = nullptr;
void  (*BindRenderbuffer)(GLenum,GLuint)                            = nullptr;
void  (*RenderbufferStorage)(GLenum,GLenum,GLsizei,GLsizei)         = nullptr;
void  (*FramebufferRenderbuffer)(GLenum,GLenum,GLenum,GLuint)       = nullptr;

static bool s_Loaded = false;

bool LoadFunctions()
{
    if (s_Loaded) return true;

    Load(ShaderSource,          "glShaderSource");
    Load(CreateShader,          "glCreateShader");
    Load(CompileShader,         "glCompileShader");
    Load(GetShaderiv,           "glGetShaderiv");
    Load(GetShaderInfoLog,      "glGetShaderInfoLog");
    Load(DeleteShader,          "glDeleteShader");
    Load(CreateProgram,         "glCreateProgram");
    Load(AttachShader,          "glAttachShader");
    Load(LinkProgram,           "glLinkProgram");
    Load(GetProgramiv,          "glGetProgramiv");
    Load(GetProgramInfoLog,     "glGetProgramInfoLog");
    Load(UseProgram,            "glUseProgram");
    Load(DeleteProgram,         "glDeleteProgram");
    Load(GetUniformLocation,    "glGetUniformLocation");
    Load(Uniform1i,             "glUniform1i");
    Load(Uniform1f,             "glUniform1f");
    Load(Uniform2f,             "glUniform2f");
    Load(Uniform3f,             "glUniform3f");
    Load(Uniform4f,             "glUniform4f");
    Load(UniformMatrix4fv,      "glUniformMatrix4fv");
    Load(GenBuffers,            "glGenBuffers");
    Load(DeleteBuffers,         "glDeleteBuffers");
    Load(BindBuffer,            "glBindBuffer");
    Load(BufferData,            "glBufferData");
    Load(BufferSubData,         "glBufferSubData");
    Load(GenVertexArrays,       "glGenVertexArrays");
    Load(DeleteVertexArrays,    "glDeleteVertexArrays");
    Load(BindVertexArray,       "glBindVertexArray");
    Load(EnableVertexAttribArray,"glEnableVertexAttribArray");
    Load(VertexAttribPointer,   "glVertexAttribPointer");
    Load(ActiveTexture,         "glActiveTexture");
    Load(GenerateMipmap,        "glGenerateMipmap");
    Load(GenFramebuffers,       "glGenFramebuffers");
    Load(DeleteFramebuffers,    "glDeleteFramebuffers");
    Load(BindFramebuffer,       "glBindFramebuffer");
    Load(FramebufferTexture2D,  "glFramebufferTexture2D");
    Load(CheckFramebufferStatus,"glCheckFramebufferStatus");
    Load(GenRenderbuffers,      "glGenRenderbuffers");
    Load(DeleteRenderbuffers,   "glDeleteRenderbuffers");
    Load(BindRenderbuffer,      "glBindRenderbuffer");
    Load(RenderbufferStorage,   "glRenderbufferStorage");
    Load(FramebufferRenderbuffer,"glFramebufferRenderbuffer");

    s_Loaded = (CreateShader != nullptr && GenBuffers != nullptr && GenVertexArrays != nullptr);
    GOOD_LOG_INFO("GL", "Functions loaded: {}", s_Loaded ? "OK" : "FAILED");
    return s_Loaded;
}

bool IsLoaded() { return s_Loaded; }

}} // namespace Good::GL