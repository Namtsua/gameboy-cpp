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
#define LCD_STATUS_LOCATION 0xFF41
#define SCROLL_Y_LOCATION 0xFF42
#define SCROLL_X_LOCATION 0xFF43
#define SCAN_LINE_LOCATION 0xFF44
#define SCAN_LINE_COMPARE_LOCATION 0xFF45
#define BG_PALETTE_LOCATION 0xFF47
#define WINDOW_Y_LOCATION 0xFF4A
#define OBJECT_PALETTE_0_LOCATION 0xFF48
#define OBJECT_PALETTE_1_LOCATION 0xFF49
#define WINDOW_X_LOCATION 0xFF4B // minus 7


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
	void render_tiles();
	void render_sprites();
	void set_display_uniform_colour(const byte& r, const byte& g, const byte& b);
	bool get_draw_flag() const;
	void set_draw_flag(const bool& enable);
	void set_lcd_status();
	void read_lcd_control();
	void read_lcd_status();
	void read_scroll_y();
	void read_scroll_x();
	void read_scanline();
	byte read_scanline_compare() const;
	byte read_object_palette_0() const;
	byte read_object_palette_1() const;
	byte read_window_y() const;
	byte read_window_x() const;
	void write_lcd_control();
	void write_lcd_status();
	void write_scroll_y(const byte& value);
	void write_scroll_x(const byte& value);
	void write_scanline_compare(const byte& value);
	void write_bg_palette(const byte& value);
	void write_object_palette_0(const byte& value);
	void write_object_palette_1(const byte& value);
	void write_window_y(const byte& value);
	void write_window_x(const byte& value);
	byte display[DISPLAY_WIDTH][DISPLAY_HEIGHT][RGBA_LENGTH];

private:
	CPU* m_cpu;
	MMU* m_mmu;
	byte gpu_mode_clock;
	byte scroll_x;
	byte scroll_y;
	// LCD Control Register
	byte bg_display_enable;
	byte sprite_display_enable;
	byte sprite_size;
	byte bg_tile_map;
	byte bg_tile_set;
	byte window_display_enable;
	byte window_tile_map;
	byte lcd_display_enable;
	// LCD Status Register
	byte gpu_mode;
	byte coincidence_flag;
	byte mode_0_hblank_interrupt;
	byte mode_1_vblank_interrupt;
	byte mode_2_oam_interrupt;
	byte lyc_ly_coincidence_interrupt;

	byte scanline;
	bool draw = false;
};