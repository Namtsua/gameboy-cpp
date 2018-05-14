#include "mmu.hpp"

MMU::MMU()
{
	clear_memory();
}

MMU::~MMU() {}

byte MMU::read_memory(const word& address)
{
	switch (address & 0xF000)
	{
		// Cartridge ROM, bank 0
	case 0x0000:
	case 0x1000:
	case 0x2000:
	case 0x3000:
		return cartridge_bank_0[address];

		//  Cartridge ROM, other banks
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
		return cartridge_other_banks[address - 0x4000];

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
			return graphics_ram[address - 0xFE00];

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