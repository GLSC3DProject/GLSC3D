#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<math.h>
#include "glsc3d.h"

#define WINDOW_SIZE_X     (600)
#define WINDOW_SIZE_Y     (600)

int main()
{
	printf("あ\n");
	g_init("GRAPH",WINDOW_SIZE_X,WINDOW_SIZE_Y);
	
	g_def_scale_2D(0,-2, 2, -2, 2,0, 0,WINDOW_SIZE_X, WINDOW_SIZE_Y);
	
	int i_time;
	
	char TEXT2[1024];
	time_t time_val;
	
	for(i_time = 0;;i_time++)
	{
		g_cls();
		
		g_sel_scale_2D(0);
		
		g_text_color(1, 1, 1, 1);
		time_val = time(0);
		strftime(TEXT2, 1023, "%Y %m/%d\n%H:%M:%S", localtime(&time_val));
		unsigned x = 10, y = 20, dy = 25;
		g_text_font(G_IPA_GOTHIC_PROPORTIONAL, 24);
		g_text_standard(x, y, "abcdefghijklmnopqrstuvwxyz");
		g_text_standard(x, y += dy, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		g_text_standard(x, y += dy, "Hello, World! with IPA_GOTHIC_PROPORTIONAL");
		g_text_standard(x, y += dy, TEXT2);
		
		y += dy;
		
		g_text_font(G_IPA_GOTHIC, 24);
		g_text_standard(x, y += dy, "abcdefghijklmnopqrstuvwxyz");
		g_text_standard(x, y += dy, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		g_text_standard(x, y += dy, "Hello, World! with IPA_GOTHIC");
		g_text_standard(x, y += dy, TEXT2);
		
		y += dy;
		
		g_text_font(G_IPA_MINCHO_PROPORTIONAL, 24);
		g_text_standard(x, y += dy, "abcdefghijklmnopqrstuvwxyz");
		g_text_standard(x, y += dy, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		g_text_standard(x, y += dy, "Hello, World! with IPA_MINCHO_PROPORTIONAL");
		g_text_standard(x, y += dy, TEXT2);
		
		y += dy;
		
		g_text_font(G_IPA_MINCHO, 24);
		g_text_standard(x, y += dy, "abcdefghijklmnopqrstuvwxyz");
		g_text_standard(x, y += dy, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		g_text_standard(x, y += dy, "Hello, World! with IPA_MINCHO");
		g_text_standard(x, y += dy, "あHello, World! with IPA_MINCHO");
		g_text_standard(x, y += dy, TEXT2);
		
		x = 10, y += 2*dy;
		for(unsigned i = 20; i < 46; ++i){
			char c = 'a' + i - 20;
			g_text_font(G_IPA_GOTHIC, i);
			g_text_standard(x += i/2, y, "%c", c);
		}
		for(unsigned i = 46; i < 46 + 5; ++i){
			char c = '0' + i - 46;
			g_text_font(G_IPA_GOTHIC, i);
			g_text_standard(x += i/2, y, "%c", c);
		}
		y += 50;
		g_text_font(0, 50);
		g_text_standard(10, y, "!\"#$%&'()`@;:+*<>,.{}[]\\");
		
		g_sleep(0.01);
		g_finish();

 	}
	return 0;
}

