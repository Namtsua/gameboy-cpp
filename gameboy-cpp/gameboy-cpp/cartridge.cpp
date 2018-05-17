#include "cartridge.hpp"

Cartridge::Cartridge(MMU* mmu)
{
	m_mmu = mmu;
}

bool Cartridge::load(const string& rom)
{
	ifstream in_file(rom, ios::in | ios::binary);
	if (!in_file.is_open())
	{
		fprintf(stderr, "Failed to open ROM file\n");
		return false;
	}

	in_file.seekg(0, in_file.end); // go to end of file
	int length = in_file.tellg(); // get file size
	in_file.seekg(0, in_file.beg); // go back to beginning of file
	byte* rom_space = m_mmu->get_rom_space();
	in_file.read((char *)rom_space, length); // read program into memory
	in_file.close();

	return true;
}