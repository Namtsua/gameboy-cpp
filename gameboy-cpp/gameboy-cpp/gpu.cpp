#include "gpu.hpp"

GPU::GPU()
{
	clear_display();
}

GPU::~GPU() {}

void GPU::cycle()
{
	switch (gpu_mode)
	{
	case HORIZONTAL_BLANK: break;
	case VERTICAL_BLANK: break;
	case OAM_READ_MODE: break;
	case VRAM_READ_MODE: break;
	}
	draw = true;
}

void GPU::clear_display()
{
	for (int i = 0; i < DISPLAY_WIDTH; ++i)
		for (int j = 0; j < DISPLAY_HEIGHT; ++j)
			for (int k = 0; k < RGBA_LENGTH; ++k)
				display[i][j][k] = 0xFF;
}

void GPU::set_display_uniform_colour(const byte& r, const byte& g, const byte& b)
{
	for (int i = 0; i < DISPLAY_WIDTH; ++i)
		for (int j = 0; j < DISPLAY_HEIGHT; ++j)
			for (int k = 0; k < RGBA_LENGTH; ++k)
			{
				if (k == 3)
				{
					display[i][j][k] = 0xFF;
				}
				else
				{
					display[i][j][0] = r;
					display[i][j][1] = g;
					display[i][j][2] = b;
				}
			}
}

bool GPU::get_draw_flag() const
{
	return draw;
}

void GPU::set_draw_flag(const bool& enable)
{
	draw = enable;
}