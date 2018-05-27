#include "gpu.hpp"

GPU::GPU()
{
	clear_display();
}

GPU::~GPU() {}

void GPU::cycle()
{

}

void GPU::clear_display() 
{
	for (int i = 0; i < GB_DISPLAY_WIDTH; ++i)
		for (int j = 0; j < GB_DISPLAY_WIDTH; ++j)
			for (int k = 0; k < RGBA_LENGTH; ++k)
				display[i][j][k] = 0x0;
}

void GPU::set_display_uniform_colour(const byte& colour) 
{
	for (int i = 0; i < GB_DISPLAY_WIDTH; ++i)
		for (int j = 0; j < GB_DISPLAY_WIDTH; ++j)
			for (int k = 0; k < RGBA_LENGTH; ++k)
				display[i][j][k] = colour;
}