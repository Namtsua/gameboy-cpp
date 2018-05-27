#pragma once
#include "common.hpp"

#define GB_DISPLAY_HEIGHT 144
#define GB_DISPLAY_WIDTH 160
#define RGBA_LENGTH 4

class GPU
{
public:
	GPU();
	~GPU();
	void cycle();
	void clear_display();
	void set_display_uniform_colour(const byte& colour);

private:
	byte display[GB_DISPLAY_WIDTH][GB_DISPLAY_HEIGHT][RGBA_LENGTH];
};