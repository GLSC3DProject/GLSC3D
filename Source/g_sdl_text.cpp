#include "glsc3d_private_ext.h"
#include <stdarg.h>
#include <ft2build.h>
#include FT_FREETYPE_H
//#include <freetype/freetype.h>

GLuint g_texture, g_sampler;
GLuint g_quad_vao, g_quad_vbo;
static FT_Library library;
static FT_Face face = 0;

void g_activate_texture_mode();

//struct
//{
//	G_COLOR color;
//	const char *font_type;
//	unsigned int font_size;
//}           glsc3D_g_def_text[TotalDisplayNumber], current_font;
//
//const struct
//{
//	G_FONT_ID id;
//	const unsigned long long *memory;
//	const unsigned size;
//}glsc3D_g_embedded_text[] = {
//	{G_IPA_GOTHIC, g_font_ipag_data, g_font_ipag_size},
//	{G_IPA_GOTHIC_PROPORTIONAL, g_font_ipagp_data, g_font_ipagp_size},
//	{G_IPA_MINCHO, g_font_ipam_data, g_font_ipam_size},
//	{G_IPA_MINCHO_PROPORTIONAL, g_font_ipamp_data, g_font_ipamp_size}
//};

#ifdef __APPLE__
#define FONT_FILE "/System/Library/Fonts/Menlo.ttc"
#endif

#ifdef __linux__
#define FONT_FILE
#endif

#ifdef _WIN32
#define FONT_FILE "C:/Windows/Fonts/Meiryo.ttc"
#endif

void g_text_init()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &g_texture);
	glGenSamplers(1, &g_sampler);

	glSamplerParameteri(g_sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(g_sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(g_sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glSamplerParameteri(g_sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenVertexArrays(1, &g_quad_vao);
	glBindVertexArray(g_quad_vao);

	glEnableVertexAttribArray(0);

	glGenBuffers(1, &g_quad_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, g_quad_vbo);

	float vertices[] = {0, 0,  1, 0,  0, 1,  1, 1};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	if (FT_Error error = FT_Init_FreeType(&library)){
		fprintf(stderr, "Unable to init Freetype.Abort.\nError : %d\n", error);
		g_quit();
	}

	if (FT_Error error = FT_New_Face(library, FONT_FILE, 0, &face)) {
		printf("Unable to load font. Abort.\nError: %d\n", error);
		g_quit();
	}

	if (FT_Error error = FT_Set_Pixel_Sizes(face, 0, 24)) {
		printf("Unable to set font size. Abort.\nError: %d\n", error);
		g_quit();
	}
}

void g_text_render(double x, double y, const char *str)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_texture);
	glBindSampler(0, g_sampler);
	g_activate_texture_mode();
	glBindVertexArray(g_quad_vao);

	//unsigned int prev = 0;
	while (FT_UInt c = (FT_Byte)*str) {
		FT_UInt char_code = 0;
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
			char_code = ((c & 0x1F) << 6) | (str[1] & 0x3F);
			str += 2;
		}
		else if (c < 0xF0) {
			// 3-byte, 16-bit character
			char_code = ((c & 0x1F) << 12) | (str[1] & 0x3F) << 6 | (str[2] & 0x3F);
			str += 3;
		}
		else {
			// Invalid
			printf("Invalid text.\n");
			return;
		}

		FT_Error error = FT_Load_Char(face, char_code, FT_LOAD_RENDER);
		if (error) continue;

		FT_Bitmap& bitmap = face->glyph->bitmap;

		glViewport(x, glsc3D_height - (y + bitmap.rows), bitmap.width, bitmap.rows);
		glBindVertexArray(g_quad_vao);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bitmap.width, bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap.buffer);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		x += face->glyph->advance.x / 64;
		//y += face->glyph->advance.y / 64;
		//prev = index;
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	if (g_scale_dim_flag == G_3D)
		g_sel_scale_3D(get_scale_id_number);
	else
		g_sel_scale_2D(get_scale_id_number);
}

void g_text_standard(double x, double y, const char *str, ...)
{
	char buf[256];
	va_list ap;
	va_start(ap, str);

	vsnprintf(buf, sizeof(buf), str, ap);

	g_text_render(x, y, buf);

	va_end(ap);
}

//void g_text_3D_virtual(double x,double y,double z, const char *str, ...)
//{
//	char buf[256], *pbuf = buf;
//	va_list ap;
//	va_start(ap, str);
//
//	vsnprintf(buf, 255, str, ap);
//
//	glEnd();
//	glDisable(GL_LIGHTING);
//	glColor4fv(&g_current_text_color.r);
//	glRasterPos3d(x,y,z);
//	g_text_redering(pbuf);
//
//	va_end(ap);
//	glEnd();
//}
//void g_text_2D_virtual(double x,double y, const char *str, ...)
//{
//	char buf[256], *pbuf = buf;
//	va_list ap;
//	va_start(ap, str);
//
//	vsnprintf(buf, 255, str, ap);
//
//	glEnd();
//	glDisable(GL_LIGHTING);
//	glColor4fv(&g_current_text_color.r);
//	glRasterPos2d(x, y);
//	g_text_redering(pbuf);
//
//	va_end(ap);
//	glEnd();
//}
//void g_text_font(G_FONT_ID id, unsigned int font_size)
//{
//	g_text_font_core((const char *)id, font_size);
//}
//
//void g_text_font_core(const char *font_type, unsigned int font_size)
//{
//	//current_font.color = g_current_text_color;
//
//	//current_font.font_type = font_type;
//	//current_font.font_size = font_size;
//
//	if(face && FT_Done_Face(face)){
//		fprintf(stderr, "error occurring while destroying face object.\n");
//		exit(1);
//	}
//	if((unsigned int)font_type < 4){
//		if(FT_New_Memory_Face(library, (const FT_Byte *)glsc3D_g_embedded_text[(int)font_type].memory, glsc3D_g_embedded_text[(int)font_type].size, 0, &face)){
//			fprintf(stderr, "error occurring while reading font from memory.\n");
//			exit(1);
//		}
//	}else{
//		if(FT_New_Face(library, font_type, 0, &face)){
//			fprintf(stderr, "error occurring while reading font file.\n");
//			exit(1);
//		}
//	}
//	if(FT_Set_Pixel_Sizes(face, 0, font_size)){
//		fprintf(stderr, "error occurring while setting font size.\n");
//		exit(1);
//	}
//}
/*

void g_text_color_s(G_COLOR color)
{
	g_current_text_color = color;
}

void g_text_color(double r,double g,double b,double a)
{
	g_text_color_s(g_color_core(r,g,b,a));
}

void g_def_text_core(int id, double r, double g, double b, double a, char * font_type, unsigned font_size)
{
	glsc3D_g_def_text[id].color = g_color_core(r, g, b, a);
	glsc3D_g_def_text[id].font_type = font_type;
	glsc3D_g_def_text[id].font_size = font_size;
}

void g_def_text(int id, double r, double g, double b, double a, int font, unsigned int font_size)
{
	g_def_text_core(id, r, g, b, a, (char*)(long long)font, font_size);
}

void g_sel_text(int id)
{
	g_text_color_s(glsc3D_g_def_text[id].color);
	g_text_font_core(glsc3D_g_def_text[id].font_type, glsc3D_g_def_text[id].font_size);
}
*/
