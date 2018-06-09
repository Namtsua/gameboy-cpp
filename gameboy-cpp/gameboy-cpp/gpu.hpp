#pragma once
#include "common.hpp"
#include "cpu.hpp"
#include "mmu.hpp"

#define HORIZONTAL_BLANK 0
#define VERTICAL_BLANK 1
#define OAM_READ_MODE 2
#define VRAM_READ_MODE 3

#define HORIZONTAL_BLANK_CYCLES 204
#define VERTICAL_BLANK_CYCLES 456
#define OAM_READ_MODE_CYCLES 80
#define VRAM_READ_MODE_CYCLES 172

#define LCD_CONTROL_LOCATION 0xFF40
#define SCROLL_Y_LOCATION 0xFF42
#define SCROLL_X_LOCATION 0xFF43
#define SCAN_LINE_LOCATION 0xFF44
#define BG_PALETTE_LOCATION 0xFF47


using namespace std;
using namespace Constants;

class GPU
{
public:
	GPU(CPU* cpu, MMU* mmu);
	~GPU();
	void cycle();
	void reset();
	void clear_display();
	void clear_tileset();
	void render_scanline();
	void set_display_uniform_colour(const byte& r, const byte& g, const byte& b);
	bool get_draw_flag() const;
	void set_draw_flag(const bool& enable);
	byte read_lcd_control() const;
	byte read_scroll_y() const;
	byte read_scroll_x() const;
	byte read_scanline() const;
	void write_lcd_control();
	void write_scroll_y(const byte& value);
	void write_scroll_x(const byte& value);
	void write_bg_palette(const byte& value);
	byte display[DISPLAY_WIDTH][DISPLAY_HEIGHT][RGBA_LENGTH];

private:
	CPU* m_cpu;
	MMU* m_mmu;
	byte gpu_mode;
	byte gpu_mode_clock;
	byte scroll_x;
	byte scroll_y;
	byte switch_bg;
	byte switch_sprites;
	byte sprites_size;
	byte bg_tile_map;
	byte bg_tile_set;
	byte switch_window;
	byte window_tile_map;
	byte switch_display;

	int line;
	bool draw = false;
};