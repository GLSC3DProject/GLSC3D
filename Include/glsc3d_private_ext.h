#ifndef GLSC3D_PRIVATE_EXT_H
#define GLSC3D_PRIVATE_EXT_H

#include "glsc3d_private.h"

#define G_USE_CORE_PROFILE

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef _WIN32
	#define EXTERN_DECL_GL_FUNC(Type, Name) extern Type Name;
	#define DECL_GL_FUNC(Type, Name) Type Name;
	#define INIT_GL_FUNC(Type, Name) Name = (Type)wglGetProcAddress(#Name);

	#define EMIT_GL_FUNCTIONS(Action) \
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
	//Action(PFNGLGENRENDERBUFFERSPROC,			glGenRenderbuffers) \
	//Action(PFNGLBINDRENDERBUFFERPROC,			glBindRenderbuffer) \
	//Action(PFNGLRENDERBUFFERSTORAGEPROC,		glRenderbufferStorage) \
	//Action(PFNGLGENFRAMEBUFFERSPROC,			glGenFramebuffers) \
	//Action(PFNGLBINDFRAMEBUFFERPROC,			glBindFramebuffer) \
	//Action(PFNGLFRAMEBUFFERRENDERBUFFERPROC,	glFramebufferRenderbuffer)

	EMIT_GL_FUNCTIONS(EXTERN_DECL_GL_FUNC);
#endif

#ifdef G_USE_CORE_PROFILE

	enum { G_UNIFORM_MATRICES, G_UNIFORM_LIGHTS, G_NUM_UNIFORMS };

	void g_update_uniform(GLuint index, GLsizei size, GLvoid *data);

#endif // G_USE_CORE_PROFILE

#ifdef __cplusplus
}
#endif

#endif
