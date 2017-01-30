#include "glsc3d_private.h"
#include <stdio.h>

#define INPUT_KEY_MAX (128) // This is enough size to store ASCII character.
							//And, I'll store special keys and mouse buttons
							//in place of contoroll character.

unsigned char input_key[INPUT_KEY_MAX];
struct 
{
	int x, y;
}click_pos;

//filter function
//non-accepted key is mapped 0
unsigned char keyboard_map(unsigned char code)
{
#ifdef VERBOSE
	printf("raw ascii key %x\n", code);
#endif
	if(0x20 <= code && code <= 0x7f)
		return code;
	switch(code)
	{
		//fall through
		case 0x08:
		case 0x09:
		case 0x0a:
		case 0x0d:
		case 0x1b:
			return code;
		default:
			return G_NONE;
	}
}

void keyboard(unsigned char key, int x, int y)
{
	//dammy code for warning unused variable
	x = y;
	input_key[keyboard_map(key)] |= 1;
}

unsigned char special_map(unsigned char code)
{
#ifdef VERBOSE
	printf("raw special key %x\n", code);
#endif
	switch(code)
	{
		case GLUT_KEY_F1:
			return G_KEY_F1;
		case GLUT_KEY_F2:
			return G_KEY_F2;
		case GLUT_KEY_F3:
			return G_KEY_F3;
		case GLUT_KEY_F4:
			return G_KEY_F4;
		case GLUT_KEY_F5:
			return G_KEY_F5;
		case GLUT_KEY_F6:
			return G_KEY_F6;
		case GLUT_KEY_F7:
			return G_KEY_F7;
		case GLUT_KEY_F8:
			return G_KEY_F8;
		case GLUT_KEY_F9:
			return G_KEY_F9;
		case GLUT_KEY_F10:
			return G_KEY_F10;
		case GLUT_KEY_F11:
			return G_KEY_F11;
		case GLUT_KEY_F12:
			return G_KEY_F12;
		case GLUT_KEY_LEFT:
			return G_KEY_LEFT;
		case GLUT_KEY_UP:
			return G_KEY_UP;
		case GLUT_KEY_RIGHT:
			return G_KEY_RIGHT;
		case GLUT_KEY_DOWN:
			return G_KEY_DOWN;
		case GLUT_KEY_PAGE_UP:
			return G_KEY_PAGE_UP;
		case GLUT_KEY_PAGE_DOWN:
			return G_KEY_PAGE_DOWN;
		case GLUT_KEY_HOME:
			return G_KEY_HOME;
		case GLUT_KEY_END:
			return G_KEY_END;
		case GLUT_KEY_INSERT:
			return G_KEY_INSERT;
		default:
			return G_KEY_INVALID;
	}
}

void special(int key, int x, int y)
{
	//dammy code for warning unused variable
	x = y;
	input_key[special_map(key)] |= 1;
}

void mouse(int button, int state, int x, int y)
{
#ifdef VERBOSE
	printf("raw mouse parameter b:%x s:%x x:%d y:%d\n", button, state, x, y);
#endif
	int key_index = button == GLUT_LEFT_BUTTON ? G_MOUSE_LEFT : 
						button == GLUT_MIDDLE_BUTTON ? G_MOUSE_MIDDLE : 
							button == GLUT_RIGHT_BUTTON ? G_MOUSE_RIGHT : G_KEY_INVALID;
	if(key_index != G_KEY_INVALID)
	{
		click_pos.x = x, click_pos.y = y;
		if(state == GLUT_DOWN)
			input_key[key_index] |= 1;
		if(state == GLUT_UP)
			input_key[key_index] &= -2;
	}
}

void g_input_init(void)
{
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMouseFunc(mouse);
}

void update_input_key_state()
{
	for(int i = 0; i < INPUT_KEY_MAX; ++i)
		input_key[i] <<= 1;
	input_key[G_MOUSE_LEFT]   |= (input_key[G_MOUSE_LEFT]   >> 1) & 1;
	input_key[G_MOUSE_MIDDLE] |= (input_key[G_MOUSE_MIDDLE] >> 1) & 1;
	input_key[G_MOUSE_RIGHT]  |= (input_key[G_MOUSE_RIGHT]  >> 1) & 1;
}

void g_get_input()
{
	update_input_key_state();
}

G_INPUT_STATE g_key_state(G_KEY_CODE key)
{
	switch((input_key[key] >> 1) & 3)
	{
		case G_NONE:
			return G_NONE;
		case G_DOWN:
			return G_DOWN;
		case G_UP:
			return G_UP;
		case G_REPEAT:
			return G_REPEAT;
		default:
			return G_NONE;
	}
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
		"Page Up",
		"Page Down",
		"Home",
		"End",
		"Left Click",
		"Middle Click",
		"Right Click",
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
