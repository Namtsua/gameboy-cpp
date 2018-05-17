#include "cartridge.hpp"

Cartridge::Cartridge(const string& file_path)
{
	rom_path = file_path.c_str();
}

bool Cartridge::load()
{
	ifstream in_file(rom_path, ios::in | ios::binary);
	if (!in_file.is_open())
	{
		fprintf(stderr, "Failed to open ROM file\n");
		return false;
	}

	in_file.seekg(0, in_file.end); // go to end of file
	int length = in_file.tellg(); // get file size
	in_file.seekg(0, in_file.beg); // go back to beginning of file
	//in_file.read((char *)&m_cpu.memory[Constants::PROGRAM_START], length); // read program into memory
	in_file.close();

	return true;
}