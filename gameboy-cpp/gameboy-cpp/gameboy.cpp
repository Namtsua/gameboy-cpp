#include "gameboy.hpp"


MMU* mmu = NULL;
Cartridge* cart = NULL;
CPU* cpu = NULL;
GPU* gpu = NULL;

GameBoy::GameBoy() {}
GameBoy::~GameBoy() {}

void GameBoy::start()
{
	mmu = new MMU();
	cart = new Cartridge(mmu);
	cart->load("./roms/DMG_ROM.bin");
	cpu = new CPU(mmu);
	gpu = new GPU(cpu, mmu);
}

void GameBoy::cycle()
{
	cpu->cycle();
	gpu->cycle();
}

GPU* GameBoy::get_gpu() const
{
	return gpu;
}