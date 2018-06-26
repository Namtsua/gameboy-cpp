#pragma once
#include "registers.hpp"
#include "mmu.hpp"

class CPU
{
public:
	CPU(MMU* mmu);
	~CPU();
	void cycle();
	byte decode(const word& pc);
	void CB(const byte& opcode);
	void handle_interrupts();
	std::pair<byte, byte> get_clock_cycles() const;
	void print_registers() const;

private:
	MMU * m_mmu;
	Registers m_registers;
	Register dst;
	Register reg;
	CombinedRegister cr_dst;
	byte src;
	word cr_src;
	bool carry = false;
	word pc_step = 0x00;
	byte immediate_u8 = 0x00;
	ibyte signed_immediate_u8 = 0x00;
	word immediate_u16 = 0x0000;
	iword signed_immmediate_u16 = 0x00;
	byte memory_value_u8;
	word memory_value_u16;
	byte bit;
	bool interrupt_master_enable = false;
	bool halt_enable = false;
	bool stop_enable = false;
	bool debug_mode = false;
};