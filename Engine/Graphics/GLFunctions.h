#pragma once
#include "../Core/Core.h"

#if defined(GOOD_PLATFORM_WINDOWS)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <gl/GL.h>
#elif defined(GOOD_PLATFORM_LINUX)
    #include <GL/gl.h>
#endif

typedef char     GLchar;
typedef intptr_t GLsizeiptr;
typedef intptr_t GLintptr;

#ifndef GL_FRAGMENT_SHADER
    #define GL_FRAGMENT_SHADER          0x8B30
    #define GL_VERTEX_SHADER            0x8B31
    #define GL_COMPILE_STATUS           0x8B81
    #define GL_LINK_STATUS              0x8B82
    #define GL_INFO_LOG_LENGTH          0x8B84
    #define GL_ARRAY_BUFFER             0x8892
    #define GL_ELEMENT_ARRAY_BUFFER     0x8893
    #define GL_UNIFORM_BUFFER           0x8A11
    #define GL_STATIC_DRAW              0x88E4
    #define GL_DYNAMIC_DRAW             0x88E8
    #define GL_STREAM_DRAW              0x88E0
    #define GL_TEXTURE0                 0x84C0
    #define GL_CLAMP_TO_EDGE            0x812F
    #define GL_RGBA8                    0x8058
    #define GL_R8                       0x8229
    #define GL_RG8                      0x822B
    #define GL_RG                       0x8227
    #define GL_RED                      0x1903
    #define GL_FRAMEBUFFER              0x8D40
    #define GL_COLOR_ATTACHMENT0        0x8CE0
    #define GL_DEPTH_ATTACHMENT         0x8D00
    #define GL_DEPTH_STENCIL_ATTACHMENT 0x821A
    #define GL_RENDERBUFFER             0x8D41
    #define GL_DEPTH24_STENCIL8         0x88F0
    #define GL_FRAMEBUFFER_COMPLETE     0x8CD5
    #define GL_MIRRORED_REPEAT          0x8370
#endif

namespace Good { namespace GL
{
    bool LoadFunctions();
    bool IsLoaded();

    extern void  (*ShaderSource)(GLuint, GLsizei, const GLchar**, const GLint*);
    extern GLuint(*CreateShader)(GLenum);
    extern void  (*CompileShader)(GLuint);
    extern void  (*GetShaderiv)(GLuint, GLenum, GLint*);
    extern void  (*GetShaderInfoLog)(GLuint, GLsizei, GLsizei*, GLchar*);
    extern void  (*DeleteShader)(GLuint);
    extern GLuint(*CreateProgram)();
    extern void  (*AttachShader)(GLuint, GLuint);
    extern void  (*LinkProgram)(GLuint);
    extern void  (*GetProgramiv)(GLuint, GLenum, GLint*);
    extern void  (*GetProgramInfoLog)(GLuint, GLsizei, GLsizei*, GLchar*);
    extern void  (*UseProgram)(GLuint);
    extern void  (*DeleteProgram)(GLuint);
    extern GLint (*GetUniformLocation)(GLuint, const GLchar*);
    extern void  (*Uniform1i)(GLint, GLint);
    extern void  (*Uniform1f)(GLint, GLfloat);
    extern void  (*Uniform2f)(GLint, GLfloat, GLfloat);
    extern void  (*Uniform3f)(GLint, GLfloat, GLfloat, GLfloat);
    extern void  (*Uniform4f)(GLint, GLfloat, GLfloat, GLfloat, GLfloat);
    extern void  (*UniformMatrix4fv)(GLint, GLsizei, GLboolean, const GLfloat*);
    extern void  (*GenBuffers)(GLsizei, GLuint*);
    extern void  (*DeleteBuffers)(GLsizei, const GLuint*);
    extern void  (*BindBuffer)(GLenum, GLuint);
    extern void  (*BufferData)(GLenum, GLsizeiptr, const void*, GLenum);
    extern void  (*BufferSubData)(GLenum, GLintptr, GLsizeiptr, const void*);
    extern void  (*GenVertexArrays)(GLsizei, GLuint*);
    extern void  (*DeleteVertexArrays)(GLsizei, const GLuint*);
    extern void  (*BindVertexArray)(GLuint);
    extern void  (*EnableVertexAttribArray)(GLuint);
    extern void  (*VertexAttribPointer)(GLuint, GLint, GLenum, GLboolean, GLsizei, const void*);
    extern void  (*ActiveTexture)(GLenum);
    extern void  (*GenerateMipmap)(GLenum);
    extern void  (*GenFramebuffers)(GLsizei, GLuint*);
    extern void  (*DeleteFramebuffers)(GLsizei, const GLuint*);
    extern void  (*BindFramebuffer)(GLenum, GLuint);
    extern void  (*FramebufferTexture2D)(GLenum, GLenum, GLenum, GLuint, GLint);
    extern GLenum(*CheckFramebufferStatus)(GLenum);
    extern void  (*GenRenderbuffers)(GLsizei, GLuint*);
    extern void  (*DeleteRenderbuffers)(GLsizei, const GLuint*);
    extern void  (*BindRenderbuffer)(GLenum, GLuint);
    extern void  (*RenderbufferStorage)(GLenum, GLenum, GLsizei, GLsizei);
    extern void  (*FramebufferRenderbuffer)(GLenum, GLenum, GLenum, GLuint);
}}