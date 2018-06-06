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

private:
	Registers r;
};