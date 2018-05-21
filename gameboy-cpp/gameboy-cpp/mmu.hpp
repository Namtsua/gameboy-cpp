#pragma once
#include "common.hpp"
#define bank_size 0x8000
#define graphics_ram_size 0x2000
#define external_ram_size 0x2000
#define working_ram_size 0x2000
#define ram_size 0x2000
#define working_ram_shadow_size 0x1E00
#define sprite_info_size 0xA0
#define mapped_io_size 0x80
#define zero_page_size 0x80

class MMU
{
public:
	MMU();
	~MMU();
	byte read_memory(const word& address) const;
	word read_memory_u16(const word& address) const;
	void write_memory(const word& address, const byte& value);
	void write_memory_u16(const word& address, const word& value);
	byte* get_rom_space();
	


	inline void clear_memory()
	{
		for (int i = 0; i < bank_size; ++i)
		{
			cartridge_bank[i] = 0x0;
		}

		for (int i = 0; i < ram_size; ++i)
		{
			graphics_ram[i] = 0x0;
			cartridge_external_ram[i] = 0x0;
			working_ram[i] = 0x0;
		}

		for (int i = 0; i < working_ram_shadow_size; ++i)
			working_ram_shadow[i] = 0x0;

		for (int i = 0; i < sprite_info_size; ++i)
			graphics_sprite_info[i] = 0x0;

		for (int i = 0; i < mapped_io_size; ++i)
			memory_mapped_io[i] = 0x0;

		for (int i = 0; i < zero_page_size; ++i)
			zero_page_ram[i] = 0x0;
	}

private:
	byte cartridge_bank[bank_size];
	byte graphics_ram[graphics_ram_size];
	byte cartridge_external_ram[external_ram_size];
	byte working_ram[working_ram_size];
	byte working_ram_shadow[working_ram_shadow_size];
	byte graphics_sprite_info[sprite_info_size];
	byte memory_mapped_io[mapped_io_size];
	byte zero_page_ram[zero_page_size];
};