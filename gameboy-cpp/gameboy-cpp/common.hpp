#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <tuple>

typedef uint8_t byte;
typedef int8_t ibyte;
typedef uint16_t word;
typedef int16_t iword;

enum Register
{
	R_B,
	R_C,
	R_D,
	R_E,
	R_H,
	R_L,
	R_F,
	R_A,
};

enum CombinedRegister
{
	R_BC,
	R_DE,
	R_HL,
	R_AF,
};

enum Keys
{
	KEY_DEFAULT,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_A,
	KEY_B,
	KEY_START,
	KEY_SELECT,
	KEY_COUNT
};

namespace Constants
{
	const int REGISTER_COUNT = 8;
	const int DISPLAY_WIDTH = 160;
	const int DISPLAY_HEIGHT = 144;
	const int RGBA_LENGTH = 4;
//	const int SCREEN_WIDTH = 640;
//	const int SCREEN_HEIGHT = 480;
	const int TOTAL_SIZE = DISPLAY_WIDTH * DISPLAY_HEIGHT * RGBA_LENGTH;
}