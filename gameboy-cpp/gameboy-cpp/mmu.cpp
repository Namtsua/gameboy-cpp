#include "mmu.hpp"

// TODO: Read ROM header for information about bank switching

MMU::MMU()
{
	clear_memory();
	initialize_bootloader();
}

MMU::~MMU() {}

byte MMU::read_memory(const word& address) const
{
	switch (address & 0xF000)
	{
		// Cartridge ROM, bank 0
	case 0x0000:
	case 0x1000:
	case 0x2000:
	case 0x3000:
		return cartridge_bank[address];

		//  Cartridge ROM, other banks
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
		return cartridge_bank[address];

		//	Graphics RAM
	case 0x8000:
	case 0x9000:
		return graphics_ram[address - 0x8000];

		// Cartridge External RAM
	case 0xA000:
	case 0xB000:
		return cartridge_external_ram[address - 0xA000];

		// Working RAM
	case 0xC000:
	case 0xD000:
		return working_ram[address - 0xC000];

		// Working RAM (shadow)
	case 0xE000:
		return working_ram_shadow[address - 0xE000];

	case 0xF000:
	{
		// Working RAM (shadow)
		if (address >= 0xF000 && address <= 0xFDFF)
			return working_ram_shadow[address - 0xF000];

		// Graphics RAM
		if (address >= 0xFE00 && address <= 0xFE9F)
			return graphics_sprite_info[address - 0xFE00];

		// Memory-mapped IO
		if (address >= 0xFF00 && address <= 0xFF7F)
			return memory_mapped_io[address - 0xFF00];

		// Zero-page RAM
		if (address >= 0xFF80 && address <= 0xFFFF)
			return zero_page_ram[address - 0xFF80];
	}

	default:
		fprintf(stderr, "Unknown memory address!");
		exit(1);
	}
}

word MMU::read_memory_u16(const word& address) const
{
//	return read_memory(address) << 8 | read_memory(address + 1);
	return read_memory(address) | (read_memory(address + 1) << 8);
}

void MMU::write_memory(const word& address, const byte& value)
{
	switch (address & 0xF000)
	{
		// Cartridge ROM, bank 0
	case 0x0000:
	case 0x1000:
	case 0x2000:
	case 0x3000:
		cartridge_bank[address] = value;
		break;

		//  Cartridge ROM, other banks
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
		cartridge_bank[address] = value;
		break;

		//	Graphics RAM
	case 0x8000:
	case 0x9000:
		graphics_ram[address - 0x8000] = value;
		break;

		// Cartridge External RAM
	case 0xA000:
	case 0xB000:
		cartridge_external_ram[address - 0xA000] = value;
		break;

		// Working RAM
	case 0xC000:
	case 0xD000:
		working_ram[address - 0xC000] = value;
		break;

		// Working RAM (shadow) (also reflect in Working RAM)
	case 0xE000:
		working_ram_shadow[address - 0xE000] = value;
		working_ram[address - 0xE000] = value;
		break;

	case 0xF000:
	{
		// Working RAM (shadow) (also reflect in Working RAM)
		if (address >= 0xF000 && address <= 0xFDFF)
		{
			working_ram_shadow[address - 0xF000] = value;
			working_ram[address - 0xF000] = value;
		}

		// Graphics RAM
		else if (address >= 0xFE00 && address <= 0xFE9F)
			graphics_sprite_info[address - 0xFE00] = value;

		// Memory-mapped IO
		else if (address >= 0xFF00 && address <= 0xFF7F)
		{
			// reset current scanline if write attempt
			if (address == 0xFF44)
			{
				memory_mapped_io[address - 0xFF00] = 0;
			}
			else if (address == 0xFF46)
			{
				direct_memory_address_transfer(value);
			}
			else
			{
				memory_mapped_io[address - 0xFF00] = value;
			}
		}

		else if (address >= 0xFF80 && address <= 0xFFFF)
			zero_page_ram[address - 0xFF80] = value;
	}
	break;

	default:
		fprintf(stderr, "Unknown memory address!");
		exit(1);
	}
}

// check on this
void MMU::write_memory_u16(const word& address, const word& value)
{
	write_memory(address, value >> 8);
	write_memory(address + 1, value & 0xFF);
}

void MMU::direct_memory_address_transfer(const byte& value)
{
	// Source address will be divided by 100, so shift by 8 to multiply by 100
	word address = value << 8;
	// Write to OAM (sprite RAM)
	for (int i = 0x0; i < 0xA0; ++i)
		write_memory(0xFE00 + i, read_memory(address + i));
}

byte* MMU::get_rom_space()
{
	return cartridge_bank;
}

void MMU::set_current_scanline(const byte& value)
{
	memory_mapped_io[current_scanline - 0xFF00] = value;
}

void MMU::increment_current_scanline()
{
	++memory_mapped_io[current_scanline - 0xFF00];
}