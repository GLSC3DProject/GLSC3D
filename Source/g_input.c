#include "glsc3d_private.h"
#include <stdio.h>

#define INPUT_KEY_MAX (128) // This is enough size to store ASCII character.
							//And, I'll store special keys and mouse buttons
							//in place of control character.

G_INPUT_STATE input_key[INPUT_KEY_MAX];
struct 
{
	int x, y;
}click_pos;

void g_keyboard_func(G_KEY_CODE_CONSTANT key, G_INPUT_STATE state)
{
	if (state == G_DOWN)
		input_key[key] |= 1;
	if (state == G_UP)
		input_key[key] &= ~2;
}

void g_mouse_func(G_KEY_CODE_CONSTANT button, G_INPUT_STATE state, int x, int y)
{
#ifdef VERBOSE
	printf("raw mouse parameter b:%x s:%x x:%d y:%d\n", button, state, x, y);
#endif
	click_pos.x = (int)(x / g_screen_scale_factor);
	click_pos.y = (int)(y / g_screen_scale_factor);
	if (state == G_DOWN)
		input_key[button] |= 1;
	if (state == G_UP)
		input_key[button] &= -2;
}

void update_input_key_state()
{
	for(int i = 0; i < INPUT_KEY_MAX; ++i)
		input_key[i] = (input_key[i] & 1) << 1;
//	input_key[G_MOUSE_LEFT]   |= (input_key[G_MOUSE_LEFT]   >> 1) & 1;
//	input_key[G_MOUSE_MIDDLE] |= (input_key[G_MOUSE_MIDDLE] >> 1) & 1;
//	input_key[G_MOUSE_RIGHT]  |= (input_key[G_MOUSE_RIGHT]  >> 1) & 1;
}

void g_get_input()
{
	update_input_key_state();
}

G_INPUT_STATE g_key_state(G_KEY_CODE key)
{
	return (G_INPUT_STATE)((input_key[key] >> 1) & 3);
}

G_INPUT_STATE g_input_state(G_KEY_CODE key, int *x, int *y)
{
	if(x != NULL)
		*x = click_pos.x;
	if(y != NULL)
		*y = click_pos.y;

	return g_key_state(key);
}

char *g_key_code_string[] = 
	{
		"Invalid", //0x00
		"Left Click",
		"Middle Click",
		"Right Click",
		"Page Up",
		"Page Down",
		"Home",
		"End",
		"Back Space",
		"Tab",
		"Enter",
		"Insert",
		"Not Assigned",
		"Enter",
		"F1",
		"F2",
		"F3", //0x10
		"F4",
		"F5",
		"F6",
		"F7",
		"F8",
		"F9",
		"F10",
		"F11",
		"F12",
		"Not Assined",
		"Escape",
		"Left Arrow",
		"Up Arrow",
		"Right Arrow",
		"Down Arrow",
		"Space", //0x20
		"!",
		"\"",
		"#",
		"$",
		"%",
		"&",
		"'",
		"(",
		")",
		"*",
		"+",
		",",
		"-",
		".",
		"/",
		"0", //0x30
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",
		":",
		";",
		"<",
		"=",
		">",
		"?",
		"@", //0x40
		"A",
		"B",
		"C",
		"D",
		"E",
		"F",
		"G",
		"H",
		"I",
		"J",
		"K",
		"L",
		"M",
		"N",
		"O",
		"P", //0x50
		"Q",
		"R",
		"S",
		"T",
		"U",
		"V",
		"W",
		"X",
		"Y",
		"Z",
		"[",
		"\\",
		"]",
		"^",
		"_",
		"`", //0x60
		"a",
		"b",
		"c",
		"d",
		"e",
		"f",
		"g",
		"h",
		"i",
		"j",
		"k",
		"l",
		"m",
		"n",
		"o",
		"p", //0x70
		"q",
		"r",
		"s",
		"t",
		"u",
		"v",
		"w",
		"x",
		"y",
		"z",
		"{",
		"|",
		"}",
		"~",
		"Delete"
	};

