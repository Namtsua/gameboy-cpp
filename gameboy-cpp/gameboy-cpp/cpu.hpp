#pragma once
#include "registers.hpp"
#include "mmu.hpp"

class CPU
{
public:
	CPU(MMU* mmu);
	~CPU();
	void cycle();
	word decode();


private:
	MMU * m_mmu;
	Registers m_registers;
};