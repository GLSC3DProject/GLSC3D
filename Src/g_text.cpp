#include "glsc3d_3_private.h"
#include <stdarg.h>
#include <ft2build.h>
#include FT_FREETYPE_H

GLuint g_texture;

#ifdef G_USE_CORE_PROFILE
GLuint g_sampler, g_quad_vao;
#else
GLuint g_quad_vbo;
#endif

G_COLOR g_current_text_color;
float g_current_text_size = 0;

static FT_Library library;
static FT_Face face = 0;

struct G_TEXT_APPEARANCE
{
	G_COLOR color;
	const char *font_type;
	float font_size;

	void select()
	{
		g_text_color_s(color);
		g_text_font_core(font_type);
		g_text_size(font_size);
	}
};

G_TEXT_APPEARANCE glsc3D_g_def_text[TotalDisplayNumber];

#define DEFAULT_FONT_NAME "Consola.ttf"

#ifdef __linux__
#define DEFAULT_FONT_FILE "/usr/share/fonts/opentype/noto/" DEFAULT_FONT_NAME
#endif

#ifdef _WIN32
#define DEFAULT_FONT_FILE "C:/Windows/Fonts/" DEFAULT_FONT_NAME
#endif

void g_text_init()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &g_texture);
#ifdef G_USE_CORE_PROFILE
	glGenSamplers(1, &g_sampler);

	glSamplerParameteri(g_sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(g_sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(g_sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glSamplerParameteri(g_sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#else
	glBindTexture(GL_TEXTURE_2D, g_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);
#endif

	float vertices[] = {-1, -1,  +1, -1,  -1, +1,  +1, +1};

#ifdef G_USE_CORE_PROFILE
	glGenVertexArrays(1, &g_quad_vao);
	glBindVertexArray(g_quad_vao);

	glEnableVertexAttribArray(0);

	GLuint g_quad_vbo;
	glGenBuffers(1, &g_quad_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, g_quad_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
#else
	glGenBuffers(1, &g_quad_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, g_quad_vbo);

	glEnableClientState(GL_VERTEX_ARRAY);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexPointer(2, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif

	if (FT_Error error = FT_Init_FreeType(&library)){
		fprintf(stderr, "Unable to init Freetype. Abort.\nError : %d\n", error);
		g_quit();
	}

#ifdef __APPLE__
	char default_font[256];
	strcpy(default_font, getenv("HOME"));
	strcat(default_font, "/Library/Fonts/" DEFAULT_FONT_NAME);

	g_text_font_core(default_font);
#else
	g_text_font_core(DEFAULT_FONT_FILE);
#endif

	g_text_size(24);
}

static void g_text_render(double x, double y, const char *str)
{
	g_use_program(g_texture_program);
	glBindTexture(GL_TEXTURE_2D, g_texture);

#ifdef G_USE_CORE_PROFILE
	glBindSampler(0, g_sampler);
	glBindVertexArray(g_quad_vao);
#else
	glBindBuffer(GL_ARRAY_BUFFER, g_quad_vbo);
#endif

	glUniform1i(g_texture_sampler_location, 0);
	glUniform4fv(g_texture_color_location, 1, &g_current_text_color.r);

	int font_size = (int)(g_current_text_size * g_screen_scale_factor);

	if (FT_Error error = FT_Set_Char_Size(face, 0, font_size * 64, 0, 0)) {
		printf("Unable to set font size.\nError: %d\n", error);
	}

	int physical_x = (int)(x * g_screen_scale_factor);
	int physical_y = (int)(y * g_screen_scale_factor);

	while (FT_UInt c = (FT_Byte)*str) {
		// Decode UTF-8
		FT_UInt char_code;
		if (c < 0x80) {
			// 1-byte, 7-bit character (ASCII)
			char_code = c;
			str += 1;
		}
		else if (c < 0xC0) {
			// Invalid
			printf("Invalid text.\n");
			return;
		}
		else if (c < 0xE0) {
			// 2-byte, 11-bit character
			char_code = (c & 0x1F) << 6 | (str[1] & 0x3F);
			str += 2;
		}
		else if (c < 0xF0) {
			// 3-byte, 16-bit character
			char_code = (c & 0x1F) << 12 | (str[1] & 0x3F) << 6 | (str[2] & 0x3F);
			str += 3;
		}
		else {
			// Invalid
			printf("Invalid text.\n");
			return;
		}

		FT_Error error = FT_Load_Char(face, char_code, FT_LOAD_RENDER);
		if (error) continue;

		FT_GlyphSlot glyph = face->glyph;
		FT_Bitmap& bitmap = glyph->bitmap;

		int left = physical_x + glyph->bitmap_left;
		int bottom = glsc3D_height + glyph->bitmap_top - bitmap.rows - physical_y;

		glViewport(left, bottom, bitmap.width, bitmap.rows);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bitmap.width, bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap.buffer);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		physical_x += glyph->advance.x / 64;
		physical_y += glyph->advance.y / 64;
	}

	glBindTexture(GL_TEXTURE_2D, 0);

#ifdef G_USE_CORE_PROFILE
	glBindVertexArray(0);
#else
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif

	glsc3D_inner_scale[g_current_scale_id].select();
}

void g_text_standard_v(double x, double y, const char *format, va_list args)
{
	char buf[256];
	vsnprintf(buf, sizeof(buf), format, args);

	g_text_render(x, y, buf);
}

void g_text_3D_virtual_v(double x, double y, double z, const char *format, va_list args)
{
	G_SCALE& scale = glsc3D_inner_scale[g_current_scale_id];

	G_VECTOR4 p = G_VECTOR4(x, y, z, 1) * scale.camera.view * scale.camera.proj;
	float std_x = scale.screen.x + 0.5f * (1 + p.x / p.w) * scale.screen.width;
	float std_y = scale.screen.y + 0.5f * (1 - p.y / p.w) * scale.screen.height;

	g_text_standard_v(std_x, std_y, format, args);
}

void g_text_standard(double x, double y, const char *format, ...)
{
	va_list args;
	va_start(args, format);

	g_text_standard_v(x, y, format, args);

	va_end(args);
}

void g_text_3D_virtual(double x, double y, double z, const char *format, ...)
{
	va_list args;
	va_start(args, format);

	g_text_3D_virtual_v(x, y, z, format, args);

	va_end(args);
}

void g_text_2D_virtual(double x, double y, const char *format, ...)
{
	va_list args;
	va_start(args, format);

	g_text_3D_virtual_v(x, y, 0, format, args);

	va_end(args);
}

void g_text_color_s(G_COLOR color)
{
	g_current_text_color = color;
}

void g_text_color(float r, float g, float b, float a)
{
	g_current_text_color = G_COLOR(r, g, b, a);
}

void g_text_font_core(const char *font_type)
{
	if (font_type == NULL) return;

	if (face != NULL) {
		if (FT_Error error = FT_Done_Face(face)) {
			fprintf(stderr, "Unable to destroy previous face object.\nError: %d\n", error);
		}
		face = NULL;
	}
	if (FT_Error error = FT_New_Face(library, font_type, 0, &face)) {
		fprintf(stderr, "Unable to load font'%s'.\nError: %d\n", font_type, error);
	}
}

void g_text_size(float size)
{
	g_current_text_size = size;
}

void g_def_text_core(int id, float r, float g, float b, float a, const char *font_type, float font_size)
{
	glsc3D_g_def_text[id].color = G_COLOR(r, g, b, a);
	glsc3D_g_def_text[id].font_type = font_type;
	glsc3D_g_def_text[id].font_size = font_size;
}

//void g_def_text(int id, float r, float g, float b, float a, int font, float font_size)
void g_def_text(int id, float r, float g, float b, float a, float font_size)
{
	g_def_text_core(id, r, g, b, a, NULL, font_size);
}

void g_sel_text(int id)
{
	glsc3D_g_def_text[id].select();
}
