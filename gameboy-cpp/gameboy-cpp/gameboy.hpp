#pragma once
#include "common.hpp"
#include "registers.hpp"
#include "cpu.hpp"
#include "mmu.hpp"
#include "gpu.hpp"
#include "cartridge.hpp"

class GameBoy
{
	public:
		GameBoy();
		~GameBoy();
		void start();
		void cycle();
		GPU* get_gpu() const;

private:
	Registers r;
};