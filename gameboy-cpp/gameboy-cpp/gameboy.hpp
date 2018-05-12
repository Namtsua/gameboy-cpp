#pragma once
#include "common.hpp"
#include "registers.hpp"
#include "cpu.hpp"

class GameBoy
{
	public:
		GameBoy();
		~GameBoy();

private:
	Registers r;
};