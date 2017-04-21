#ifdef __linux__

#include "glsc3d_3_private.h"

static GLuint frame_buffer;
static GLuint color_buffer, depth_buffer;

void g_init_off_screen_rendering()
{
	//version check
#ifndef GL_VERSION_3_3
#warning your environment may be not able to use off-screen rendering, sorry.
#endif
	GLint major_version = 0, minor_version = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &major_version);
	glGetIntegerv(GL_MINOR_VERSION, &minor_version);

	if(major_version < 3 || (major_version == 3 && minor_version < 3))
		fprintf(stderr, "GLSC3D : Sorry, your environment may be not able to use off-screen rendering,\nyour OpenGL implementation is : %s\n", glGetString(GL_VERSION));

	G_DECL_INIT_GLEXT(PFNGLGENRENDERBUFFERSPROC, glGenRenderbuffers);
	G_DECL_INIT_GLEXT(PFNGLBINDRENDERBUFFERPROC, glBindRenderbuffer);
	G_DECL_INIT_GLEXT(PFNGLRENDERBUFFERSTORAGEPROC, glRenderbufferStorage);
	G_DECL_INIT_GLEXT(PFNGLGENFRAMEBUFFERSPROC, glGenFramebuffers);
	G_DECL_INIT_GLEXT(PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer);
	G_DECL_INIT_GLEXT(PFNGLFRAMEBUFFERRENDERBUFFERPROC, glFramebufferRenderbuffer);

	//initialize
	glGenRenderbuffers(1, &color_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, color_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, glsc3D_width, glsc3D_height);

	glGenRenderbuffers(1, &depth_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, glsc3D_width, glsc3D_height);

	glGenFramebuffers(1, &frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, color_buffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);
}

#endif