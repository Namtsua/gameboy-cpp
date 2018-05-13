#pragma once
#include "registers.hpp"

class CPU
{
public:
	CPU();
	~CPU();
	void cycle();
	word decode();


private:
	Registers m_registers;
};