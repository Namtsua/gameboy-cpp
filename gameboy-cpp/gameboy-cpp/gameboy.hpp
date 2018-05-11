#pragma once
#include "common.hpp"
#include "registers.hpp"

class GameBoy
{
	public:
		GameBoy();
		~GameBoy();

private:
	Registers r;
};