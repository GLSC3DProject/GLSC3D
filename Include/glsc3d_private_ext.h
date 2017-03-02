#ifndef GLSC3D_PRIVATE_EXT_H
#define GLSC3D_PRIVATE_EXT_H

#include "glsc3d_private.h"

//#define G_USE_CORE_PROFILE

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef G_USE_CORE_PROFILE

enum { G_UNIFORM_MATRICES, G_UNIFORM_LIGHTS, G_NUM_UNIFORMS };
	
void g_shader_program_init();
void g_update_uniform(GLuint index, GLsizei size, GLvoid *data);

#ifdef _WIN32
#define G_NEED_GET_GLEXT_PROC_ADDRESS

#define G_EXTERN_DECL_GLEXT(Type, Name) extern Type Name;
#define G_DECL_GLEXT(Type, Name) Type Name;
#define G_INIT_GLEXT(Type, Name) Name = (Type)wglGetProcAddress(#Name);

#define G_EMIT_GLEXT(Action) \
Action(PFNGLVERTEXATTRIBPOINTERPROC,		glVertexAttribPointer) \
Action(PFNGLENABLEVERTEXATTRIBARRAYPROC,	glEnableVertexAttribArray) \
Action(PFNGLGENVERTEXARRAYSPROC,			glGenVertexArrays) \
Action(PFNGLBINDVERTEXARRAYPROC,			glBindVertexArray) \
Action(PFNGLGENBUFFERSPROC,					glGenBuffers) \
Action(PFNGLBUFFERDATAPROC,					glBufferData) \
Action(PFNGLBUFFERSUBDATAPROC,				glBufferSubData) \
Action(PFNGLBINDBUFFERPROC,					glBindBuffer) \
Action(PFNGLCREATEPROGRAMPROC,				glCreateProgram) \
Action(PFNGLLINKPROGRAMPROC,				glLinkProgram) \
Action(PFNGLUSEPROGRAMPROC,					glUseProgram) \
Action(PFNGLCREATESHADERPROC,				glCreateShader) \
Action(PFNGLSHADERSOURCEPROC,				glShaderSource) \
Action(PFNGLCOMPILESHADERPROC,				glCompileShader) \
Action(PFNGLATTACHSHADERPROC,				glAttachShader) \
Action(PFNGLGETPROGRAMIVPROC,				glGetProgramiv) \
Action(PFNGLGETSHADERIVPROC,				glGetShaderiv) \
Action(PFNGLGETPROGRAMINFOLOGPROC,			glGetProgramInfoLog) \
Action(PFNGLGETSHADERINFOLOGPROC,			glGetShaderInfoLog) \
Action(PFNGLGETUNIFORMBLOCKINDEXPROC,		glGetUniformBlockIndex) \
Action(PFNGLUNIFORMBLOCKBINDINGPROC,		glUniformBlockBinding) \
Action(PFNGLBINDBUFFERBASEPROC,				glBindBufferBase) \
Action(PFNGLDEBUGMESSAGECALLBACKPROC,		glDebugMessageCallback)
//Action(PFNGLGENRENDERBUFFERSPROC,			glGenRenderbuffers) \
//Action(PFNGLBINDRENDERBUFFERPROC,			glBindRenderbuffer) \
//Action(PFNGLRENDERBUFFERSTORAGEPROC,		glRenderbufferStorage) \
//Action(PFNGLGENFRAMEBUFFERSPROC,			glGenFramebuffers) \
//Action(PFNGLBINDFRAMEBUFFERPROC,			glBindFramebuffer) \
//Action(PFNGLFRAMEBUFFERRENDERBUFFERPROC,	glFramebufferRenderbuffer)

G_EMIT_GLEXT(G_EXTERN_DECL_GLEXT);

#ifndef NDEBUG
#define G_ENABLE_OPENGL_DEBUG
void APIENTRY g_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *user);
#endif

#endif

#endif

#ifdef __cplusplus
}
#endif

#endif
