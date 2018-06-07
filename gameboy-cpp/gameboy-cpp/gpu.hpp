#pragma once
#include "common.hpp"

#define HORIZONTAL_BLANK 0
#define VERTICAL_BLANK 1
#define OAM_READ_MODE 2
#define VRAM_READ_MODE 3

using namespace std;
using namespace Constants;

class GPU
{
public:
	GPU();
	~GPU();
	void cycle();
	void clear_display();
	void set_display_uniform_colour(const byte& r, const byte& g, const byte& b);
	bool get_draw_flag() const;
	void set_draw_flag(const bool& enable);
	byte display[DISPLAY_WIDTH][DISPLAY_HEIGHT][RGBA_LENGTH];

private:
	byte gpu_mode;
	byte gpu_mode_clock;
	bool draw = false;
};