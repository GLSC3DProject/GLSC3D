#include "glsc3d_private_ext.h"
#include <SDL2/SDL.h>

SDL_Window*		g_window;
SDL_GLContext	g_context;

void g_sdl_init(const char *WindowName, int pos_x, int pos_y)
{
	SDL_Init(SDL_INIT_VIDEO);
	
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	
#ifdef G_USE_CORE_PROFILE
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

#ifdef G_ENABLE_OPENGL_DEBUG
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG | SDL_GL_CONTEXT_DEBUG_FLAG);
#else
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif

#endif

	g_window = SDL_CreateWindow(WindowName,
		pos_x, pos_y, glsc3D_width, glsc3D_height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	
	g_context = SDL_GL_CreateContext(g_window);
	SDL_GL_SetSwapInterval(1);
}

void g_swap_buffers()
{
	SDL_GL_SwapWindow(g_window);
}

void g_poll_events()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			g_quit();
		}
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
			g_quit();
		}

	}
}

void g_quit()
{
	SDL_GL_DeleteContext(g_context);
	SDL_DestroyWindow(g_window);
	SDL_Quit();
	exit(0);
}
