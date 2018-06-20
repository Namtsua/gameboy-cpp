#include "gpu.hpp"

GPU::GPU(CPU* cpu, MMU* mmu, SDL_Renderer* renderer)
{
	reset();
	m_cpu = cpu;
	m_mmu = mmu;
	m_renderer = renderer;
	gpu_mode = VERTICAL_BLANK;
	gpu_mode_clock = 0;
	scanline = 0;
}

GPU::~GPU() {}

void GPU::cycle()
{
	// Read LCD control
	read_lcd_control();

	// Update GPU clock if LCD is on, return if LCD is off
	if (!lcd_display_enable)
		return;

	// Update GPU mode clock
	gpu_mode_clock += m_cpu->get_clock_cycles().first;

	// Update GPU based on mode
	handle_gpu_mode();
}

void GPU::handle_gpu_mode()
{
	// Handle current GPU mode
	switch (gpu_mode)
	{
	case OAM_READ_MODE:
		// Check to enter Mode 3 (Read VRAM)
		if (gpu_mode_clock >= OAM_READ_MODE_CYCLES)
		{
			gpu_mode_clock = 0;
			gpu_mode = VRAM_READ_MODE;
		}
		break;
	case VRAM_READ_MODE:
		// Check to enter Mode 0 (Horizontal Blank)
		if (gpu_mode_clock >= VRAM_READ_MODE_CYCLES)
		{
			gpu_mode_clock = 0;
			gpu_mode = HORIZONTAL_BLANK;
			render_scanline();
			if (mode_0_hblank_interrupt)
			{
				// Send interrupt
			}
		}
		break;
	case HORIZONTAL_BLANK:
		// Check to enter either Mode 1 (Vertical Blank) or Mode 2 (Read OAM)
		if (gpu_mode_clock >= HORIZONTAL_BLANK_CYCLES)
		{
			gpu_mode_clock = 0;
			m_mmu->increment_current_scanline();
			read_scanline();

			// Check if last horizontal blank
			if (scanline == 144)
			{
				gpu_mode = VERTICAL_BLANK;
				render_frame();
				// Trigger interrupt
				if (mode_1_vblank_interrupt)
				{
					// Send interrupt
				}
			}
			else
			{
				gpu_mode = OAM_READ_MODE;
				if (mode_2_oam_interrupt)
				{
					// Send interrupt
				}
			}
		}
		break;
	case VERTICAL_BLANK:
		// Check to potentially enter Mode 2 (Read OAm)
		if (gpu_mode_clock >= VERTICAL_BLANK_CYCLES)
		{
			gpu_mode_clock = 0;
			m_mmu->increment_current_scanline();
			read_scanline();

			if (scanline > 153)
			{
				gpu_mode = OAM_READ_MODE;
				m_mmu->set_current_scanline(0);
				if (mode_2_oam_interrupt)
				{
					// Send interrupt
				}
			}
		}
		break;
	}

	// Check coincidence flag (not sure if right thing to check)
	if (read_scanline_compare() == scanline)
	{
		coincidence_flag = 0x1;
		if (lyc_ly_coincidence_interrupt)
		{
			// Request an interrupt
		}
	}
	else
	{
		coincidence_flag = 0x0;
	}

	// Update in memory
	write_lcd_status();
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
	// Read LCD control settings
	read_lcd_control();

	// Render tiles if background is displayed
	if (bg_display_enable)
		render_tiles();

	// Render sprites if sprites are displayed
	if (sprite_display_enable)
		render_sprites();
}

void GPU::render_tiles()
{
	// Read all releveant coordinates from memory
	read_display_coordinates();

	// Tiles start at 0x8000
	word tile_memory_region = TILE_LOCATION;

	// If bit is disabled, then tiles start at 0x8800
	if (!bg_tile_set_region)
		tile_memory_region += TILE_LOCATION_MODIFIER;

	// Need to get the identifier
	word tile_start_address = tile_memory_region;

	if (!bg_tile_set_region)
		tile_start_address += (/*identifier*/ +128) * TILE_SIZE;
	else
		tile_start_address += (/*identifier*/ +0) * TILE_SIZE;
}

Colour GPU::get_colour_from_palette(const byte& colour_id, const word& address)
{
	// Initialize to white in case something goes wrong
	Colour colour = WHITE;

	// Retrieve palette
	const byte& palette = m_mmu->read_memory(address);

	// Retrieve colour from palette based on id
	switch (colour_id)
	{
	case 0x00:
		colour = static_cast<Colour>(palette & 0x3);
		break;
	case 0x01:
		colour = static_cast<Colour>((palette >> 2) & 0x3);
		break;
	case 0x10:
		colour = static_cast<Colour>((palette >> 4) & 0x3);
		break;
	case 0x11:
		colour = static_cast<Colour>((palette >> 6) & 0x3);
		break;
	default:
		fprintf(stderr, "Invalid colour ID provided");
	}

	// Return retrieved colour
	return colour;
}

void GPU::render_sprites()
{
}

void GPU::render_frame()
{
	// Colour the frame pixel by pixel
	for (int i = 0; i < 144; ++i)
	{
		for (int j = 0; j < 160; ++j)
		{
			byte r = display[i][j][0];
			byte g = display[i][j][1];
			byte b = display[i][j][2];
			byte a = display[i][j][3];
			SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
			SDL_RenderDrawPoint(m_renderer, j, i);
		}
	}

	// Update window
	SDL_RenderPresent(m_renderer);
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

// Not sure if needed
void GPU::set_lcd_status()
{
	if (!lcd_display_enable)
	{
		gpu_mode_clock = 0;
		m_mmu->set_current_scanline(0);
		gpu_mode = VERTICAL_BLANK;
		write_lcd_status();
		return;
	}
}

void GPU::read_display_coordinates()
{
	read_scroll_y();
	read_scroll_x();
	read_window_y();
	read_window_x();
}

void GPU::read_lcd_control()
{
	byte lcd_control = m_mmu->read_memory(LCD_CONTROL_LOCATION);
	bg_display_enable = lcd_control & 0x1;
	sprite_display_enable = (lcd_control >> 1) & 0x1;
	sprite_size = (lcd_control >> 2) & 0x1;
	bg_tile_map_region = (lcd_control >> 3) & 0x1;
	bg_tile_set_region = (lcd_control >> 4) & 0x1;
	window_display_enable = (lcd_control >> 5) & 0x1;
	window_tile_map_region = (lcd_control >> 6) & 0x1;
	lcd_display_enable = (lcd_control >> 7) & 0x1;
}

void GPU::read_lcd_status()
{
	byte lcd_status = m_mmu->read_memory(LCD_STATUS_LOCATION);
	gpu_mode = lcd_status & 0x3;
	coincidence_flag = (lcd_status >> 2) & 0x1;
	mode_0_hblank_interrupt = (lcd_status >> 3) & 0x1;
	mode_1_vblank_interrupt = (lcd_status >> 4) & 0x1;
	mode_2_oam_interrupt = (lcd_status >> 5) & 0x1;
	lyc_ly_coincidence_interrupt = (lcd_status >> 6) & 0x1;
}

void GPU::read_scroll_y()
{
	scroll_y = m_mmu->read_memory(SCROLL_Y_LOCATION);
}

void GPU::read_scroll_x()
{
	scroll_x = m_mmu->read_memory(SCROLL_X_LOCATION);
}

void GPU::read_scanline()
{
	scanline = m_mmu->read_memory(SCAN_LINE_LOCATION);
}

byte GPU::read_scanline_compare() const
{
	return m_mmu->read_memory(SCAN_LINE_COMPARE_LOCATION);
}

byte GPU::read_object_palette_0() const
{
	return m_mmu->read_memory(OBJECT_PALETTE_0_LOCATION);
}

byte GPU::read_object_palette_1() const
{
	return m_mmu->read_memory(OBJECT_PALETTE_0_LOCATION);
}

void GPU::read_window_y()
{
	window_y = m_mmu->read_memory(WINDOW_Y_LOCATION);
}
void GPU::read_window_x()
{
	// Substraction is necessary
	window_x = m_mmu->read_memory(WINDOW_X_LOCATION) - 7;
}

void GPU::write_lcd_control()
{
	// form LCD control byte
	byte result = (lcd_display_enable << 7)
		| (window_tile_map_region << 6)
		| (window_display_enable << 5)
		| (bg_tile_set_region << 4)
		| (bg_tile_map_region << 3)
		| (sprite_size << 2)
		| (sprite_display_enable << 1)
		| (bg_display_enable);

	m_mmu->write_memory(LCD_CONTROL_LOCATION, result);
}

void GPU::write_lcd_status()
{
	// TODO enforce read/write only
	byte result = (lyc_ly_coincidence_interrupt << 6)
		| (mode_2_oam_interrupt << 5)
		| (mode_1_vblank_interrupt << 4)
		| (mode_0_hblank_interrupt << 3)
		| (coincidence_flag << 2)
		| (gpu_mode);

	m_mmu->write_memory(LCD_STATUS_LOCATION, result);
}
void GPU::write_scroll_y(const byte& value)
{
	m_mmu->write_memory(SCROLL_Y_LOCATION, value);
}

void GPU::write_scroll_x(const byte& value)
{
	m_mmu->write_memory(SCROLL_X_LOCATION, value);
}

void GPU::write_scanline_compare(const byte& value)
{
	m_mmu->write_memory(SCAN_LINE_COMPARE_LOCATION, value);
}

void GPU::write_bg_palette(const byte& value)
{
	m_mmu->write_memory(BG_PALETTE_LOCATION, value);
}

void GPU::write_object_palette_0(const byte& value)
{
	m_mmu->write_memory(OBJECT_PALETTE_0_LOCATION, value);
}

void GPU::write_object_palette_1(const byte& value)
{
	m_mmu->write_memory(OBJECT_PALETTE_1_LOCATION, value);
}

void GPU::write_window_y(const byte& value)
{
	m_mmu->write_memory(WINDOW_Y_LOCATION, value);
}

void GPU::write_window_x(const byte& value)
{
	m_mmu->write_memory(WINDOW_X_LOCATION, value);
}