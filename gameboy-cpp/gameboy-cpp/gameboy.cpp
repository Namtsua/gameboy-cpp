#include "gameboy.hpp"

GameBoy::GameBoy()
{
	MMU* mmu = new MMU();
	Cartridge* cart = new Cartridge(mmu);
	cart->load("./roms/DMG_ROM.bin");
	CPU* cpu = new CPU(mmu);
	while(1)
		cpu->cycle();
}

GameBoy::~GameBoy()
{

}