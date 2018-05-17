#pragma once
#include "common.hpp"
#include "registers.hpp"
#include "cpu.hpp"
#include "mmu.hpp"
#include "cartridge.hpp"

class GameBoy
{
	public:
		GameBoy();
		~GameBoy();

private:
	Registers r;
};