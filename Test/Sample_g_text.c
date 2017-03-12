#include<stdio.h>
#include<string.h>
#include<time.h>
#include<math.h>
#include "glsc3d.h"

#ifdef _WIN32
#pragma execution_character_set("UTF-8")
#endif

#define WINDOW_SIZE_X     (320)
#define WINDOW_SIZE_Y     (320)

//void g_text_render(double x, double y, const char *str);

int main()
{
	g_init("GRAPH",WINDOW_SIZE_X,WINDOW_SIZE_Y);
	
	g_def_scale_2D(0,-2, 2, -2, 2,0, 0,WINDOW_SIZE_X, WINDOW_SIZE_Y);
	
	int i_time;
	
	char TEXT2[20];
	
	for(i_time = 0;;i_time++)
	{
		g_cls();
		
		g_sel_scale_2D(0);
		
		g_text_color(1, 1, 1, 1);
		time_t time_val = time(0);
		strftime(TEXT2, sizeof(TEXT2), "%Y/%m/%d %H:%M:%S", localtime(&time_val));

		int x = 20, y = 0, dy = 25;
		g_text_font_core(NULL, 24);
		// 1-byte characters
		g_text_standard(x, y += dy, "Hello, World!");

		// 2-byte characters
		g_text_standard(x, y += dy, "Γειά σου Κόσμε!");

		// 3-byte characters
		g_text_standard(x, y += dy, "こんにちは，世界!");

		g_text_standard(x, y += dy, TEXT2);

		g_text_standard(x, y += dy, "i_time = %d", i_time);

		y += dy;

		g_text_font_core(NULL, 12);
		g_text_standard(x, y += 12, "12pt");

		g_text_font_core(NULL, 18);
		g_text_standard(x, y += 18, "18pt");

		g_text_font_core(NULL, 24);
		g_text_standard(x, y += 24, "24pt");

		g_text_font_core(NULL, 32);
		g_text_standard(x, y += 32, "32pt");

		g_text_font_core(NULL, 48);
		g_text_standard(x, y += 48, "48pt");

//		y += dy;
//
////		g_text_font(G_IPA_MINCHO_PROPORTIONAL, 24);
//		g_text_standard(x, y += dy, "abcdefghijklmnopqrstuvwxyz");
//		g_text_standard(x, y += dy, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
//		g_text_standard(x, y += dy, "Hello, World! with IPA_MINCHO_PROPORTIONAL");
//		g_text_standard(x, y += dy, TEXT2);
//
//		y += dy;
//
////		g_text_font(G_IPA_MINCHO, 24);
//		g_text_standard(x, y += dy, "abcdefghijklmnopqrstuvwxyz");
//		g_text_standard(x, y += dy, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
//		g_text_standard(x, y += dy, "Hello, World! with IPA_MINCHO");
//		g_text_standard(x, y += dy, "あHello, World! with IPA_MINCHO");
//		g_text_standard(x, y += dy, TEXT2);
		
//		x = 10, y += 2*dy;
//		for(unsigned i = 20; i < 46; ++i){
//			char c = 'a' + i - 20;
//			g_text_font(G_IPA_GOTHIC, i);
//			g_text_standard(x += i/2, y, "%c", c);
//		}
//		for(unsigned i = 46; i < 46 + 5; ++i){
//			char c = '0' + i - 46;
//			g_text_font(G_IPA_GOTHIC, i);
//			g_text_standard(x += i/2, y, "%c", c);
//		}
//		y += 50;
//		g_text_font(0, 50);
//		g_text_standard(10, y, "!\"#$%&'()`@;:+*<>,.{}[]\\");
		
		g_sleep(1);
		g_finish();
 	}
	return 0;
}

