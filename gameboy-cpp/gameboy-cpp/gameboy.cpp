#include "gameboy.hpp"

GameBoy::GameBoy() {}

GameBoy::~GameBoy() {}

void GameBoy::start()
{
	MMU* mmu = new MMU();
	Cartridge* cart = new Cartridge(mmu);
	cart->load("./roms/DMG_ROM.bin");
	CPU* cpu = new CPU(mmu);
	GPU* gpu = new GPU();
	/*while (1)
	{
	cpu->cycle();
	gpu->cycle();
	}*/
}