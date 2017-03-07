#include "glsc3d_private.h"
#include <stdarg.h>
//#include "g_font_obj.symb"
#include <stdio.h>
#include <SDL2/SDL_ttf.h>
#include <glsc3d_private_ext.h>
//#include FT_FREETYPE_H

//static FT_Library library;
//static FT_Face face = 0;

static GLuint g_texture, g_quad_vao, g_quad_vbo;
static TTF_Font* g_font;

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

void g_text_init()
{
//	if(FT_Init_FreeType(&library)){
//		fprintf(stderr, "error occured while initializing freetype2.\n");
//		exit(1);
//	}
//	g_text_color(0, 0, 0, 1);
//	g_text_font(G_IPA_GOTHIC, 12);

	glGenTextures(1, &g_texture);

	glGenVertexArrays(1, &g_quad_vao);

	glGenBuffers(1, &g_quad_vbo);

	TTF_Init();

	g_font = TTF_OpenFont("NotoSansCJKjp-Regular.otf", 12);
	printf("%x\n", (uint)(ulong)g_font);
//	printf("Error: %s\n", SDL_GetError());
}

void g_text_redering(char *pbuf)
{
	SDL_Color color = {128, 128, 128, 255};
	SDL_Surface* s = TTF_RenderUTF8_Blended(g_font, pbuf, color);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s->w, s->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, s->pixels);
	g_activate_texture_mode();

	glBindVertexArray(g_quad_vao);
	glBindBuffer(GL_ARRAY_BUFFER, g_quad_vbo);

	float vertices[] = {
		0.0, 0.0,	0, 0,
		0.2, 0.0,	1, 0,
		0.0, 0.1,	0, 1,
		0.2, 0.1,	1, 1,
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*4, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*4, (GLvoid *)(sizeof(float)*2));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindTexture(GL_TEXTURE_2D, 0);

	SDL_FreeSurface(s);
}

void g_text_standard(double x,double y, const char *str, ...)
{
	glViewport(0, 0, glsc3D_width, glsc3D_height);

	char buf[256];
	va_list ap;
	va_start(ap, str);

	vsnprintf(buf, 255, str, ap);

	g_text_redering(buf);

	if(g_scale_dim_flag == G_3D) g_sel_scale_3D(get_scale_id_number);
	if(g_scale_dim_flag == G_2D) g_sel_scale_2D(get_scale_id_number);

	va_end(ap);
}
void g_text_3D_virtual(double x,double y,double z, const char *str, ...)
{
	char buf[256], *pbuf = buf;
	va_list ap;
	va_start(ap, str);

	vsnprintf(buf, 255, str, ap);

	glEnd();
	glDisable(GL_LIGHTING);
	glColor4fv(&g_current_text_color.r);
	glRasterPos3d(x,y,z);
	g_text_redering(pbuf);

	va_end(ap);
	glEnd();
}
void g_text_2D_virtual(double x,double y, const char *str, ...)
{
	char buf[256], *pbuf = buf;
	va_list ap;
	va_start(ap, str);

	vsnprintf(buf, 255, str, ap);

	glEnd();
	glDisable(GL_LIGHTING);
	glColor4fv(&g_current_text_color.r);
	glRasterPos2d(x, y);
	g_text_redering(pbuf);

	va_end(ap);
	glEnd();
}
/*
void g_text_font(G_FONT_ID id, unsigned int font_size)
{
	g_text_font_core((const char *)id, font_size);
}

void g_text_font_core(const char *font_type, unsigned int font_size)
{
	current_font.color = g_current_text_color;

	current_font.font_type = font_type;
	current_font.font_size = font_size;

	if(face && FT_Done_Face(face)){
		fprintf(stderr, "error occurring while destroying face object.\n");
		exit(1);
	}
	if((unsigned int)font_type < 4){
		if(FT_New_Memory_Face(library, (const FT_Byte *)glsc3D_g_embedded_text[(int)font_type].memory, glsc3D_g_embedded_text[(int)font_type].size, 0, &face)){
			fprintf(stderr, "error occurring while reading font from memory.\n");
			exit(1);
		}
	}else{
		if(FT_New_Face(library, font_type, 0, &face)){
			fprintf(stderr, "error occurring while reading font file.\n");
			exit(1);
		}
	}
	if(FT_Set_Pixel_Sizes(face, 0, font_size)){
		fprintf(stderr, "error occurring while setting font size.\n");
		exit(1);
	}
}

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