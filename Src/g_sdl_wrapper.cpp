#include "glsc3d_3_private.h"

#include <SDL2/SDL.h>

#ifndef __APPLE__

#define G_DECL_GLEXT(Type, Name) Type Name;
#define G_INIT_GLEXT(Type, Name) Name = (Type)SDL_GL_GetProcAddress(#Name);

G_EMIT_GLEXT(G_DECL_GLEXT);

// Uncomment to enable OpenGL debug messages
//#define G_ENABLE_OPENGL_DEBUG_CALLBACK

#endif // ifndef __APPLE__

void g_keyboard_event(const SDL_Keysym &keysym, G_INPUT_STATE state);
void g_mouse_event(const SDL_MouseButtonEvent &event, G_INPUT_STATE state);

#ifdef G_ENABLE_OPENGL_DEBUG_CALLBACK
void APIENTRY g_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *user);
#endif

SDL_Window*		g_window;
SDL_GLContext	g_context;

int g_window_width, g_window_height;

bool	g_highdpi_enabled;
G_UINT	g_antialiasing_level;

void g_update_drawable_size()
{
	SDL_GL_GetDrawableSize(g_window, &glsc3D_width, &glsc3D_height);

	float sx = (float)glsc3D_width / g_window_width;
	float sy = (float)glsc3D_height / g_window_height;

	g_screen_scale_factor = sx < sy ? sx : sy;
}

void g_enable_highdpi()
{
	if (SDL_WasInit(SDL_INIT_VIDEO)) {
		printf("g_enable_highdpi must be called before g_init(_core)\n");
		return;
	}

	g_highdpi_enabled = true;
}

void g_set_antialiasing(G_UINT level)
{
	if (SDL_WasInit(SDL_INIT_VIDEO)) {
		printf("g_set_antialiasing must be called before g_init(_core)\n");
		return;
	}

	if (level > 4) {
		printf("Invalid argument passed in g_set_antialiasing.\n");
		printf("Valid values are 0 (no antialiasing), 1, 2, 3 and 4.\n");
		return;
	}

	g_antialiasing_level = level;
}

void g_sdl_init(const char *WindowName, int pos_x, int pos_y, int width, int height)
{
#ifdef _WIN32
	SetProcessDPIAware();
#endif

	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG
//#ifdef G_ENABLE_OPENGL_DEBUG_CALLBACK
//		| SDL_GL_CONTEXT_DEBUG_FLAG
//#endif
//	);

	if (g_antialiasing_level > 0) {
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 1 << g_antialiasing_level);
	}

	g_window_width = width, g_window_height = height;

	Uint32 flags;
	if (WindowName != G_OFF_SCREEN) {
		flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

		if (g_highdpi_enabled) {
			flags |= SDL_WINDOW_ALLOW_HIGHDPI;

#ifdef _WIN32
			float dpi;
			SDL_GetDisplayDPI(0, NULL, &dpi, NULL);
			float scale = dpi / 96;

			width *= scale;
			height *= scale;

			if (pos_x != G_WINDOW_CENTERED) pos_x *= scale;
			if (pos_y != G_WINDOW_CENTERED) pos_y *= scale;
#endif
		}
	}
	else {
		flags = SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN;
	}
	g_window = SDL_CreateWindow(WindowName, pos_x, pos_y, width, height, flags);

	g_context = SDL_GL_CreateContext(g_window);

#ifndef __APPLE__
	G_EMIT_GLEXT(G_INIT_GLEXT);
#endif

	printf("OpenGL Version : %s\n", glGetString(GL_VERSION));

#ifdef G_ENABLE_OPENGL_DEBUG_CALLBACK
	G_DECL_INIT_GLEXT(PFNGLDEBUGMESSAGECALLBACKPROC, glDebugMessageCallback);
	glDebugMessageCallback(g_debug_callback, NULL);
#endif

	SDL_GL_SetSwapInterval(1);

	g_update_drawable_size();
	g_retina_scale_factor = g_screen_scale_factor;

#ifdef __linux__
	if (WindowName == G_OFF_SCREEN)
		g_init_off_screen_rendering();
#endif
}

void g_swap_buffers()
{
	SDL_GL_SwapWindow(g_window);
}

void g_poll_events()
{
	update_input_key_state();

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
			g_update_drawable_size();
		}
		else if (event.type == SDL_QUIT) {
			g_quit();
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				g_quit();

			g_keyboard_event(event.key.keysym, G_DOWN);
		}
		else if (event.type == SDL_KEYUP) {
			g_keyboard_event(event.key.keysym, G_UP);
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN) {
			g_mouse_event(event.button, G_DOWN);
		}
		else if (event.type == SDL_MOUSEBUTTONUP) {
			g_mouse_event(event.button, G_UP);
		}
	}
}

void g_sleep(double wait_time)
{
	SDL_Delay((Uint32)(wait_time * 1000));
}

void g_quit()
{
	SDL_GL_DeleteContext(g_context);
	SDL_DestroyWindow(g_window);
	SDL_Quit();
	exit(0);
}

G_KEY_CODE g_scancode_map(SDL_Scancode code)
{
#ifdef VERBOSE
	printf("raw special key %x\n", code);
#endif
	switch (code)
	{
	case SDL_SCANCODE_F1:
		return G_KEY_F1;
	case SDL_SCANCODE_F2:
		return G_KEY_F2;
	case SDL_SCANCODE_F3:
		return G_KEY_F3;
	case SDL_SCANCODE_F4:
		return G_KEY_F4;
	case SDL_SCANCODE_F5:
		return G_KEY_F5;
	case SDL_SCANCODE_F6:
		return G_KEY_F6;
	case SDL_SCANCODE_F7:
		return G_KEY_F7;
	case SDL_SCANCODE_F8:
		return G_KEY_F8;
	case SDL_SCANCODE_F9:
		return G_KEY_F9;
	case SDL_SCANCODE_F10:
		return G_KEY_F10;
	case SDL_SCANCODE_F11:
		return G_KEY_F11;
	case SDL_SCANCODE_F12:
		return G_KEY_F12;
	case SDL_SCANCODE_LEFT:
		return G_KEY_LEFT;
	case SDL_SCANCODE_UP:
		return G_KEY_UP;
	case SDL_SCANCODE_RIGHT:
		return G_KEY_RIGHT;
	case SDL_SCANCODE_DOWN:
		return G_KEY_DOWN;
	case SDL_SCANCODE_PAGEUP:
		return G_KEY_PAGE_UP;
	case SDL_SCANCODE_PAGEDOWN:
		return G_KEY_PAGE_DOWN;
	case SDL_SCANCODE_HOME:
		return G_KEY_HOME;
	case SDL_SCANCODE_END:
		return G_KEY_END;
	case SDL_SCANCODE_INSERT:
		return G_KEY_INSERT;
	case SDL_SCANCODE_KP_DIVIDE:
		return '/';
	case SDL_SCANCODE_KP_MULTIPLY:
		return '*';
	case SDL_SCANCODE_KP_MINUS:
		return '-';
	case SDL_SCANCODE_KP_PLUS:
		return '+';
	case SDL_SCANCODE_KP_PERIOD:
		return '.';
	case SDL_SCANCODE_KP_ENTER:
		return '\r';
	case SDL_SCANCODE_KP_0:
		return '0';
	case SDL_SCANCODE_KP_1:
		return '1';
	case SDL_SCANCODE_KP_2:
		return '2';
	case SDL_SCANCODE_KP_3:
		return '3';
	case SDL_SCANCODE_KP_4:
		return '4';
	case SDL_SCANCODE_KP_5:
		return '5';
	case SDL_SCANCODE_KP_6:
		return '6';
	case SDL_SCANCODE_KP_7:
		return '7';
	case SDL_SCANCODE_KP_8:
		return '8';
	case SDL_SCANCODE_KP_9:
		return '9';
	default:
		return G_KEY_INVALID;
	}
}

//filter function
//non-accepted key is mapped into 0
G_KEY_CODE g_keycode_map(SDL_Keycode code)
{
#ifdef VERBOSE
	printf("raw ascii key %x\n", code);
#endif
	if (0x20 <= code && code <= 0x7f)
		return (G_KEY_CODE)code;
	switch (code)
	{
	//fall through
	case 0x08:
	case 0x09:
	case 0x0a:
	case 0x0d:
	case 0x1b:
		return (G_KEY_CODE)code;
	default:
		return G_KEY_INVALID;
	}
}

void g_keyboard_event(const SDL_Keysym &keysym, G_INPUT_STATE state)
{
	G_KEY_CODE key;

	key = g_scancode_map(keysym.scancode);
	if (key != G_KEY_INVALID) {
		g_keyboard_func(key, state);
		return;
	}

	key = g_keycode_map(keysym.sym);
	if (key != G_KEY_INVALID) {
		g_keyboard_func(key, state);
		return;
	}
}

G_KEY_CODE g_mouse_button_map(Uint8 button)
{
	switch (button) {
	case SDL_BUTTON_LEFT:
		return G_MOUSE_LEFT;
	case SDL_BUTTON_MIDDLE:
		return G_MOUSE_MIDDLE;
	case SDL_BUTTON_RIGHT:
		return G_MOUSE_RIGHT;
	default:
		return G_KEY_INVALID;
	}
}

void g_mouse_event(const SDL_MouseButtonEvent &event, G_INPUT_STATE state)
{
	G_KEY_CODE button = g_mouse_button_map(event.button);

	if (button != G_KEY_INVALID)
		g_mouse_func(button, state, event.x, event.y);
}

#ifdef G_ENABLE_OPENGL_DEBUG_CALLBACK

#define DEF_CASE(value) case value: printf(#value); break

void APIENTRY g_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *user)
{
	printf("\nSource : ");
	switch (source) {
		DEF_CASE(GL_DEBUG_SOURCE_API);
		DEF_CASE(GL_DEBUG_SOURCE_WINDOW_SYSTEM);
		DEF_CASE(GL_DEBUG_SOURCE_SHADER_COMPILER);
		DEF_CASE(GL_DEBUG_SOURCE_APPLICATION);
		DEF_CASE(GL_DEBUG_SOURCE_OTHER);
	}

	printf(", Type : ");
	switch (type) {
		DEF_CASE(GL_DEBUG_TYPE_ERROR);
		DEF_CASE(GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR);
		DEF_CASE(GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR);
		DEF_CASE(GL_DEBUG_TYPE_PORTABILITY);
		DEF_CASE(GL_DEBUG_TYPE_PERFORMANCE);
		DEF_CASE(GL_DEBUG_TYPE_OTHER);
	}

	printf(", Severity : ");
	switch (severity) {
		DEF_CASE(GL_DEBUG_SEVERITY_HIGH);
		DEF_CASE(GL_DEBUG_SEVERITY_MEDIUM);
		DEF_CASE(GL_DEBUG_SEVERITY_LOW);
		DEF_CASE(GL_DEBUG_SEVERITY_NOTIFICATION);
	}

	printf("\n%s\n", message);
}

#endif