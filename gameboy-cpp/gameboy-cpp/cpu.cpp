#include "cpu.hpp"

CPU::CPU()
{
	m_registers.initialize();
}

CPU::~CPU() {}

void CPU::cycle()
{
	const word& opcode = decode();

	switch (opcode & 0xF000)
	{
		case 0x0000: break;
		case 0x1000: break;
		case 0x2000: break;
		case 0x3000: break;
		case 0x4000: break;
		case 0x5000: break;
		case 0x6000: break;
		case 0x7000: break;
		case 0x8000: break;
		case 0x9000: break;
		case 0xA000: break;
		case 0xB000: break;
		case 0xC000: break;
		case 0xD000: break;
		case 0xE000: break;
		case 0xF000: break;
		default:
			fprintf(stderr, "Unkown opcode!");
			exit(1);
	}
}

word CPU::decode()
{
	return 0x00;
}