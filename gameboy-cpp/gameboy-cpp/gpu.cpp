#include "gpu.hpp"

GPU::GPU(CPU* cpu, MMU* mmu)
{
	reset();
	m_cpu = cpu;
	m_mmu = mmu;
	gpu_mode = 0;
	gpu_mode_clock = 0;
	line;
}

GPU::~GPU() {}

void GPU::cycle()
{
	// Update GPU mode clock
	gpu_mode_clock += m_cpu->get_clock_cycles().first;

	switch (gpu_mode)
	{
	case OAM_READ_MODE:
		if (gpu_mode_clock >= OAM_READ_MODE_CYCLES)
		{
			gpu_mode_clock = 0;
			gpu_mode = VRAM_READ_MODE;
		}
		break;
	case VRAM_READ_MODE:
		if (gpu_mode_clock >= VRAM_READ_MODE_CYCLES)
		{
			gpu_mode_clock = 0;
			gpu_mode = HORIZONTAL_BLANK;
			render_scanline();
		}
		break;
	case HORIZONTAL_BLANK:
		if (gpu_mode_clock >= HORIZONTAL_BLANK_CYCLES)
		{
			gpu_mode_clock = 0;
			line += 1;

			// Check if last horizontal blank
			if (line == 144)
			{
				gpu_mode = VERTICAL_BLANK;
				draw = true;
			}
			else
			{
				gpu_mode = OAM_READ_MODE;
			}
		}
		break;
	case VERTICAL_BLANK:
		if (gpu_mode_clock >= VERTICAL_BLANK_CYCLES)
		{
			gpu_mode_clock = 0;
			line += 1;

			if (line > 153)
			{
				gpu_mode = OAM_READ_MODE;
				line = 0;
			}
		}
		break;
	}
}

void GPU::reset()
{
	clear_display();
	//clear_tileset();
}

void GPU::clear_display()
{
	for (int i = 0; i < DISPLAY_WIDTH; ++i)
		for (int j = 0; j < DISPLAY_HEIGHT; ++j)
			for (int k = 0; k < RGBA_LENGTH; ++k)
				display[i][j][k] = 0xFF;
}

void GPU::render_scanline()
{
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

byte GPU::read_lcd_control() const
{
	return m_mmu->read_memory(LCD_CONTROL_LOCATION);
}

byte GPU::read_scroll_y() const
{
	return m_mmu->read_memory(SCROLL_Y_LOCATION);
}

byte GPU::read_scroll_x() const
{
	return m_mmu->read_memory(SCROLL_X_LOCATION);
}

byte GPU::read_scanline() const
{
	return m_mmu->read_memory(SCAN_LINE_LOCATION);
}

void GPU::write_lcd_control()
{
	// form LCD control byte
	byte result = (switch_bg ? 0x1 : 0x0)
		| (switch_sprites ? 0x1 : 0x0)
		| (sprites_size ? 0x1 : 0x0)
		| (bg_tile_map ? 0x1 : 0x0)
		| (bg_tile_set ? 0x1 : 0x0)
		| (switch_window ? 0x1 : 0x0)
		| (window_tile_map ? 0x1 : 0x0)
		| (switch_display ? 0x1 : 0x0);

	m_mmu->write_memory(LCD_CONTROL_LOCATION, result);
}

void GPU::write_scroll_y(const byte& value)
{
	m_mmu->write_memory(SCROLL_Y_LOCATION, value);
}

void GPU::write_scroll_x(const byte& value)
{
	m_mmu->write_memory(SCROLL_X_LOCATION, value);
}

void GPU::write_bg_palette(const byte& value)
{
	m_mmu->write_memory(BG_PALETTE_LOCATION, value);
}