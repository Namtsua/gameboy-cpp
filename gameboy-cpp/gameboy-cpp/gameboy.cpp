#include "gameboy.hpp"

MMU* mmu = NULL;
Cartridge* cart = NULL;
CPU* cpu = NULL;
GPU* gpu = NULL;
SDL_Renderer* sdl_renderer = NULL;

GameBoy::GameBoy(SDL_Renderer* renderer)
{
	sdl_renderer = renderer;
}

GameBoy::~GameBoy() {}

void GameBoy::start()
{
	mmu = new MMU();
	cart = new Cartridge(mmu);
	cart->load("./roms/DMG_ROM.bin");
	cpu = new CPU(mmu);
	gpu = new GPU(cpu, mmu, sdl_renderer);
	// comment if not in debug
	cpu->print_registers();
}

void GameBoy::cycle()
{
	cpu->cycle();
	gpu->cycle();
	cpu->handle_interrupts();
}