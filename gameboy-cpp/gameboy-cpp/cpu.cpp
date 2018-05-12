#include "cpu.hpp"

CPU::CPU()
{
	m_registers.initialize();
}

CPU::~CPU() {}

void CPU::cycle()
{
	const word& opcode = decode();

	switch (opcode)
	{
	case 0x00: break;
	case 0x10: break;
	case 0x20: break;
	case 0x30: break;
		// LD B, <reg>
	case 0x40:	case 0x41:	case 0x42:	case 0x43:	case 0x44:	case 0x45:	case 0x47: // missing 0x46
		break;
		// LD C, <reg>
	case 0x48:	case 0x49:	case 0x4A:	case 0x4B:	case 0x4C:	case 0x4D:	case 0x4F: // missing 0x4E
		break;
		// LD D, <reg>
	case 0x50:	case 0x51:	case 0x52:	case 0x53:	case 0x54:	case 0x55:	case 0x57: // missing 0x56
		break;
		// LD E, <reg>
	case 0x58:	case 0x59:	case 0x5A:	case 0x5B:	case 0x5C:	case 0x5D:	case 0x5F: // missing 0x5E
		break;
		// LD H, <reg>
	case 0x60:	case 0x61:	case 0x62:	case 0x63:	case 0x64:	case 0x65:	case 0x67: // missing 0x66
		break;
		// LD L, <reg>
	case 0x68:	case 0x69:	case 0x6A:	case 0x6B:	case 0x6C:	case 0x6D:	case 0x6F: // missing 0x6E
		break;
		// LD A, <reg>
	case 0x78:	case 0x79:	case 0x7A:	case 0x7B:	case 0x7C:	case 0x7D:	case 0x7F: // missing 0x7E
		break;
		// ADD A, <reg>
	case 0x80:	case 0x81:	case 0x82:	case 0x83:	case 0x84:	case 0x85:	case 0x87:	// missing 0x86
		break;
		// ADC A, <reg>
	case 0x88:	case 0x89:	case 0x8A:	case 0x8B:	case 0x8C:	case 0x8D:	case 0x8F:	// missing 0x8E
		break;
		// SUB A, <reg>
	case 0x90:	case 0x91:	case 0x92:	case 0x93:	case 0x94:	case 0x95:	case 0x97:	// missing 0x96
		break;
		// SBC A, <reg>
	case 0x98:	case 0x99:	case 0x9A:	case 0x9B:	case 0x9C:	case 0x9D:	case 0x9F:	// missing 0x9E
		break;
		// AND A, <reg>
	case 0xA0:	case 0xA1:	case 0xA2:	case 0xA3:	case 0xA4:	case 0xA5:	case 0xA7:	// missing 0xA6
		break;
		// XOR A, <reg>
	case 0xA8:	case 0xA9:	case 0xAA:	case 0xAB:	case 0xAC:	case 0xAD:	case 0xAF:	// missing 0xAE
		break;
		// OR A, <reg>
	case 0xB0:	case 0xB1:	case 0xB2:	case 0xB3:	case 0xB4:	case 0xB5:	case 0xB7:	// missing 0xB6
		break;
		// CP A, <reg>
	case 0xB8:	case 0xB9:	case 0xBA:	case 0xBB:	case 0xBC:	case 0xBD:	case 0xBF:	// missing 0xBE
		break;
	case 0xC0: break;
	case 0xD0: break;
	case 0xE0: break;
	case 0xF0: break;
	default:
		fprintf(stderr, "Unkown opcode!");
		exit(1);
	}
}

word CPU::decode()
{
	return 0x00;
}