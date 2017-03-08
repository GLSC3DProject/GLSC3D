#include "glsc3d_private_ext.h"
#include <stdarg.h>
#include <SDL2/SDL_ttf.h>

GLuint g_texture, g_sampler;
GLuint g_quad_vao, g_quad_vbo;
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

#ifdef __MACOSX__
#define FONT_FILE "/System/Library/Fonts/Menlo.ttc"
#endif

#ifdef __linux__
#define FONT_FILE
#endif

#ifdef _WIN32
#define FONT_FILE "C:/Windows/Fonts/Consola.ttf"
#endif

SDL_Surface* s;
void g_text_init()
{
//	if(FT_Init_FreeType(&library)){
//		fprintf(stderr, "error occured while initializing freetype2.\n");
//		exit(1);
//	}
//	g_text_color(0, 0, 0, 1);
//	g_text_font(G_IPA_GOTHIC, 12);

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

	if (TTF_Init() < 0) {
		printf("Unable to init SDL_TTF. Abort.\nError: %s\n", SDL_GetError());
		g_quit();
	}

	g_font = TTF_OpenFont(FONT_FILE, 32);
	if (g_font == NULL) {
		printf("Unable to load font. Abort.\n");
		g_quit();
	}
	SDL_Color color = {255, 255, 128}, back = {0};
	s = TTF_RenderText_Blended(g_font, "ABC", color);
	printf("w = %d, h = %d\n", s->w, s->h);
	for (int i = 0; i < s->h; i++) {
		for (int j = 0; j < s->w; j++)
			printf("%X ", ((uint32_t *)s->pixels)[j * s->w + i] & 0xFF);
		printf("\n");
	}
	s = TTF_RenderText_Shaded(g_font, "ABC", color, back);
//	printf("w = %d, h = %d\n", s->w, s->h);
//	s.w = 256; s.h = 128;
//	s.pixels = malloc(s.w*s.h*4);
//	for (int i = 0; i < s.h; i++) {
//		for (int j = 0; j < s.w; j++)
//			((uint32_t *)s.pixels)[i * s.h + j] = ((i ^ j) & 4) * 0x303010;
//		printf("\n");
//	}
	//SDL_FreeSurface(s);

	//printf("%x\n", g_font);
}

void g_text_redering(char *pbuf)
{
	//SDL_Color color = {128, 128, 0, 255};
	//SDL_Surface* s = TTF_RenderUTF8_Blended(g_font, pbuf, color);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_texture);
	glBindSampler(0, g_sampler);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s->w, s->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, s->pixels);
	g_activate_texture_mode();

	glBindVertexArray(g_quad_vao);

	glViewport(0, 0, s->w, s->h);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindTexture(GL_TEXTURE_2D, 0);

	//SDL_FreeSurface(s);
}

void g_text_standard(double x,double y, const char *str, ...)
{
	glViewport(0, 0, glsc3D_width, glsc3D_height);

	char buf[256];
	va_list ap;
	va_start(ap, str);

	vsnprintf(buf, sizeof(buf), str, ap);

	g_text_redering(buf);

	if(g_scale_dim_flag == G_3D) g_sel_scale_3D(get_scale_id_number);
	if(g_scale_dim_flag == G_2D) g_sel_scale_2D(get_scale_id_number);

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
