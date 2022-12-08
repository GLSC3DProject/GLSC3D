#include "glsc3d_3_private.h"
#include <cstdarg>
#include <algorithm>
#include <ft2build.h>
#include FT_FREETYPE_H

#ifdef G_USE_METAL
MTL::Texture *g_texture;
#else
GLuint g_texture;

#ifdef G_USE_OPENGL_CORE_PROFILE
GLuint g_sampler, g_quad_vao;
#else
GLuint g_quad_vbo;
#endif
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

static unsigned char *g_text_buffer = nullptr;

#define USE_RELATIVE_FONT_PATH

#define DEFAULT_FONT_NAME "NotoSansCJKjp-Regular.otf"

#ifdef USE_RELATIVE_FONT_PATH

#define DEFAULT_FONT_FILE "../Install_file_and_script_and_fonts/" DEFAULT_FONT_NAME

#else

#ifdef __linux__
#define DEFAULT_FONT_FILE "/usr/share/fonts/opentype/noto/" DEFAULT_FONT_NAME
#endif

#ifdef _WIN32
#define DEFAULT_FONT_FILE "C:/Windows/Fonts/" DEFAULT_FONT_NAME
#endif

#endif

void g_text_init()
{
#ifndef G_USE_METAL
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &g_texture);
#ifdef G_USE_OPENGL_CORE_PROFILE
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

#ifdef G_USE_OPENGL_CORE_PROFILE
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
#endif

	if (FT_Error error = FT_Init_FreeType(&library)){
		fprintf(stderr, "Unable to init Freetype. Abort.\nError : %d\n", error);
		g_quit();
	}

#ifdef DEFAULT_FONT_FILE
	g_text_font_core(DEFAULT_FONT_FILE);
#else
	char default_font[256];
	strcpy(default_font, getenv("HOME"));
	strcat(default_font, "/Library/Fonts/" DEFAULT_FONT_NAME);

	g_text_font_core(default_font);
#endif
}

void g_text_buffer_size_update()
{
	free(g_text_buffer);
	g_text_buffer = g_malloc<unsigned char>(glsc3D_width * glsc3D_height);

#ifdef G_USE_METAL
	if (g_texture) g_texture->release();

	G_AUTO_RELEASE<MTL::TextureDescriptor> desc;
	desc->setWidth(glsc3D_width);
	desc->setHeight(glsc3D_height);
	desc->setPixelFormat(MTL::PixelFormatR8Unorm);
	desc->setTextureType(MTL::TextureType2D);
	desc->setStorageMode(MTL::StorageModeManaged);
	desc->setUsage(MTL::ResourceUsageRead);
	g_texture = g_device->newTexture(desc);
#endif
}

void g_text_buffer_clear()
{
	memset(g_text_buffer, 0, glsc3D_width * glsc3D_height);
}

static void g_text_buffer_write(int x, int y, int w, int h, const unsigned char *data)
{
	int X = std::min(x + w, glsc3D_width);
	int Y = std::min(y + h, glsc3D_height);

	for (int j = std::max(y, 0); j < Y; j++) {
		for (int i = std::max(x, 0); i < X; i++) {
			g_text_buffer[glsc3D_width * j + i] = data[w * (j - y) + (i - x)];
		}
	}
}

static void g_text_buffer_render(double x, double y, const char *str)
{
	int font_size = (int)(g_current_text_size * g_screen_scale_factor);

	if (face == nullptr) {
		printf("Error: font face is null.\n");
		return;
	}

	if (FT_Error error = FT_Set_Char_Size(face, 0, font_size * 64, 0, 0)) {
		printf("Unable to set font size.\nError: %d\n", error);
	}

//	int height = face->size->metrics.height >> 6;

	int position_x = (int)(x * g_screen_scale_factor);
	int position_y = (int)(y * g_screen_scale_factor);// + (face->size->metrics.ascender >> 6);

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
			char_code = (c & 0x0F) << 12 | (str[1] & 0x3F) << 6 | (str[2] & 0x3F);
			str += 3;
		}
		else if (c < 0xF8) {
			// 4-byte, 21-bit character
			char_code = (c & 0x07) << 18 | (str[1] & 0x3F) << 12 | (str[2] & 0x3F) << 6 | (str[3] & 0x3F);
			str += 4;
		}
		else {
			// Invalid
			printf("Invalid text.\n");
			return;
		}

		FT_Error error = FT_Load_Char(face, char_code, FT_LOAD_RENDER);
		if (error) continue;

		FT_GlyphSlot glyph = face->glyph;

		g_text_buffer_write(
			position_x + glyph->bitmap_left, position_y - glyph->bitmap_top,
			glyph->bitmap.width, glyph->bitmap.rows,
			glyph->bitmap.buffer);

		position_x += glyph->advance.x >> 6;
	}
}

void g_text_buffer_present()
{
	int width = glsc3D_width, height = glsc3D_height;

#ifdef G_USE_METAL
	g_command_encoder->setRenderPipelineState(g_text_program);
	G_VECTOR4 vertices[] = {
		{-1, -1, 0, (float)height},
		{-1,  1, 0, 0},
		{ 1, -1, (float)width, (float)height},
		{ 1,  1, (float)width, 0}
	};

	g_texture->replaceRegion(MTL::Region(0, 0, width, height), 0, g_text_buffer, width);
	g_command_encoder->setVertexBytes(vertices, sizeof(vertices), 0);
	g_command_encoder->setFragmentBytes(&g_current_text_color, sizeof(G_COLOR), 0);
	g_command_encoder->setFragmentTexture(g_texture, 0);
	g_set_viewport(0, 0, width, height);
	g_command_encoder->drawPrimitives(MTL::PrimitiveTypeTriangleStrip, 0UL, 4UL);
#else
	g_use_program(g_text_program);
	glBindTexture(GL_TEXTURE_2D, g_texture);

#ifdef G_USE_OPENGL_CORE_PROFILE
	glBindSampler(0, g_sampler);
	glBindVertexArray(g_quad_vao);
#else
	glBindBuffer(GL_ARRAY_BUFFER, g_quad_vbo);
#endif

	glUniform1i(g_texture_sampler_location, 0);
	glUniform4fv(g_texture_color_location, 1, &g_current_text_color.r);

	glViewport(0, 0, width, height);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, g_text_buffer);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindTexture(GL_TEXTURE_2D, 0);

#ifdef G_USE_OPENGL_CORE_PROFILE
	glBindVertexArray(0);
#else
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
#endif
}

void g_text_standard_v(double x, double y, const char *format, va_list args)
{
	char buf[256];
	vsnprintf(buf, sizeof(buf), format, args);

	g_text_buffer_render(x, y, buf);
}

void g_text_3D_virtual_v(double x, double y, double z, const char *format, va_list args)
{
	if (g_current_scale_ptr == nullptr) {
		printf("Call g_sel_scale before g_text_2D_virtual/g_text_3D_virtual\n");
		g_quit();
	}

	G_SCALE& scale = *g_current_scale_ptr;

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
