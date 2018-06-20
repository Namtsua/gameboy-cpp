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

	// Read current scanline
	read_scanline();

	// Read all relevant coordinates from memory
	read_display_coordinates();

	// Render background scanline
	render_background_scanline();

	// Render sprites if sprites are displayed
	if (sprite_display_enable)
		render_sprites();
}

void GPU::render_background_scanline()
{
	// Render background if enabled
	if (bg_display_enable)
	{
		// Background identity map start at 0x9800
		word background_tile_map_region = BACKGROUND_LOCATION;

		// If bit 3 is set, then background identity map starts at 0x9C00
		if (bg_tile_map_region)
			background_tile_map_region += BACKGROUND_LOCATION_MODIFIER;

		// Background tile data
		word background_tile_data_region = TILE_LOCATION;

		// If bit 4 is set, then we read from 0x8000 and the identifier is unsigned
		if (bg_and_window_tile_map_region)
			background_tile_map_region += BACKGROUND_LOCATION_MODIFIER;

		// Calculate the y coordinate from current scanline and scroll_y
		const byte& background_y_coordinate = scanline + scroll_y;

		// Calculate tile number by dividing by 8 (# of lines in a map) and MOD by 32 (# of tiles in BG map)
		const byte& background_y_tile = (background_y_coordinate / 8) % 32;

		// Calculate tile offset (MOD by number of pixels in a tile);
		const byte& background_y_tile_offset = background_y_coordinate % 8;

		// Render the rest of the line
		for (int x = 0; x < DISPLAY_WIDTH; ++x)
		{
			// Calculate the x coordinate from current iteration and scroll_x
			const byte& background_x_coordinate = scroll_x + x;

			// Calculate tile offset
			const byte& background_x_tile_offset = background_x_coordinate % 8;

			// Calculate the x tile number (same as before)
			const byte& background_x_tile = (background_x_coordinate / 8) % 32;

			// Calculate address by starting with the appropriate memory location, go to the appropriate y tile and then the appropriate x tile
			const word& background_tile_address = background_tile_map_region + (background_y_tile * 32) + background_x_tile;

			// Pointer to appropriate starting address
			word tile_start_address = background_tile_map_region;

			// Calculate start address (identifier is unsigned if bit 4 is enabled, signed otherwise)
			if (bg_and_window_tile_map_region)
			{
				// Background tile number
				const byte& background_tile_identifier = m_mmu->read_memory(background_tile_address);
				tile_start_address += background_tile_identifier * TILE_SIZE;
			}
			else
			{
				// Background tile number
				const ibyte& background_tile_identifier = m_mmu->read_memory(background_tile_address);
				tile_start_address += static_cast<ibyte>(background_tile_identifier) * TILE_SIZE;
			}

			// Need to factor y offset (each line is two bytes long)
			tile_start_address += (background_y_tile_offset * 2);

			// Read tile data from memory
			byte data_1 = m_mmu->read_memory(tile_start_address);
			byte data_2 = m_mmu->read_memory(tile_start_address + 1);

			// Get colour id by finding the values at the corresponding x offset
			byte colour_id = (((data_2 >> background_x_tile_offset) & 0x1) << 1) | ((data_1 >> background_x_tile_offset) & 0x1);

			// Get colour based on palette
			Colour colour_to_draw = get_colour_from_palette(colour_id);

			byte red = 0x00;
			byte green = 0x00;
			byte blue = 0x00;
			byte alpha = 0xFF;

			switch (colour_to_draw)
			{
			case WHITE:
				red = 0xFF; green = 0xFF; blue = 0xFF;
				break;
			case LIGHT_GREY:
				red = 0xCC; green = 0xCC; blue = 0xCC;
				break;
			case DARK_GREY:
				red = 0x77; green = 0x77; blue = 0x77;
				break;
			case BLACK:
				red = 0x00; green = 0x00; blue = 0x00;
				break;
			default:
				fprintf(stderr, "Unknown Game Boy colour receieved from palette");
				break;
			}

			// Draw on display
			display[x][scanline][0] = red;
			display[x][scanline][1] = green;
			display[x][scanline][2] = blue;
			display[x][scanline][3] = alpha;
		}
	}
	else
	{
		// If background is disabled, then we render a blank screen (i.e. white)
		set_display_uniform_colour(0x0, 0x0, 0x0);
	}
}

Colour GPU::get_colour_from_palette(const byte& colour_id)
{
	// Initialize to white in case something goes wrong
	Colour colour = WHITE;

	// Retrieve palette
	const byte& palette = m_mmu->read_memory(BG_PALETTE_LOCATION);

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
	bg_and_window_tile_map_region = (lcd_control >> 4) & 0x1;
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
		| (bg_and_window_tile_map_region << 4)
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