#include "glsc3d_private.h"
#include <stdio.h>

// This is enough size to store ASCII character.
// Special keys and mouse buttons are stored in place of control characters.
#define INPUT_KEY_MAX (128)

G_INPUT_STATE input_key[INPUT_KEY_MAX];

struct { int x, y; } click_pos;

void g_keyboard_func(G_KEY_CODE key, G_INPUT_STATE state)
{
	if (state == G_DOWN)
		input_key[key] |= 1;
	//if (state == G_UP)
	//	input_key[key] &= ~2;
	else
		input_key[key] = state;
}

void g_mouse_func(G_KEY_CODE button, G_INPUT_STATE state, int x, int y)
{
#ifdef VERBOSE
	printf("raw mouse parameter b:%x s:%x x:%d y:%d\n", button, state, x, y);
#endif
	click_pos.x = (int)(x * g_retina_scale_factor / g_screen_scale_factor);
	click_pos.y = (int)(y * g_retina_scale_factor / g_screen_scale_factor);

	if (state == G_DOWN)
		input_key[button] |= 1;
	//if (state == G_UP)
	//	input_key[button] &= ~2;
	else
		input_key[button] = state;
}

void update_input_key_state()
{
	for(int i = 0; i < INPUT_KEY_MAX; ++i)
		input_key[i] = input_key[i] & G_DOWN ? G_REPEAT : G_NONE;
}

G_INPUT_STATE g_key_state(G_KEY_CODE key)
{
//	return ((input_key[key] >> 1) & 3);
	return input_key[key];
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

