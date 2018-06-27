#pragma once
#include "registers.hpp"
#include "mmu.hpp"

#define INTERRUPT_FLAG_LOCATION 0xFF0F
#define INTERRUPT_ENABLE_LOCATION 0xFFFF

#define V_BLANK_SERVICE_LOCATION 0x40
#define LCD_SERVICE_LOCATION 0x48
#define TIMER_SERVICE_LOCATION 0x50
#define JOYPAD_SERVICE_LOCATION 0x60

#define V_BLANK_INTERRUPT_MASK 0x1
#define LCD_INTERRUPT_MASK 0x2
#define TIMER_INTERRUPT_MASK 0x4
#define SERIAL_INTERRUPT_MASK 0x8
#define JOYPAD_INTERRUPT_MASK 0x10


class CPU
{
public:
	CPU(MMU* mmu);
	~CPU();
	void cycle();
	byte decode(const word& pc);
	void CB(const byte& opcode);
	void handle_interrupts();
	void request_interrupt(Interrupt interrupt);
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