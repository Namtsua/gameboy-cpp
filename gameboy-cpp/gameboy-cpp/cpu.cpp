#include "cpu.hpp"

//#define LD_REG_REG(dst,src) m_registers.set_register(dst,m_registers.get_register(src)); m_registers.increment_clock_cycles(4,1)
//#define ADD_REG_REG(dst,src) m_registers.register_addition(dst, m_registers.get_register(src)); m_registers.increment_clock_cycles(4,1)
//#define SUB_REG_REG(dst,src) m_registers.register_subtraction(dst, m_registers.get_register(src)); m_registers.increment_clock_cycles(4,1)
//#define AND_REG_REG(dst,src) m_registers.register_bitwise_and(dst, m_registers.get_register(src)); m_registers.increment_clock_cycles(4,1)
//#define XOR_REG_REG(dst,src) m_registers.register_bitwise_xor(dst, m_registers.get_register(src)); m_registers.increment_clock_cycles(4,1)
//#define OR_REG_REG(dst,src) m_registers.register_bitwise_or(dst, m_registers.get_register(src)); m_registers.increment_clock_cycles(4,1)

CPU::CPU()
{
	m_registers.initialize();
}

CPU::~CPU() {}

void CPU::cycle()
{
	const word& opcode = decode();
	Register dst;
	byte src;
	bool carry = false;

	switch (opcode)
	{
		// NOP
	case 0x00:
		m_registers.increment_clock_cycles(4, 1);
		break;
	case 0x10: break;
	case 0x20: break;
	case 0x2F: // CPL
		m_registers.complement_register_A();
		m_registers.increment_clock_cycles(4, 1);
		break;
	case 0x30: break;
		// INC (8-bit)
	case 0x04: case 0x0C: case 0x14: case 0x1C: case 0x24: case 0x2C: case 0x3C:
		dst = static_cast<Register>(opcode >> 3 & 0x7);
		m_registers.increment_register(dst);
		m_registers.increment_clock_cycles(4, 1);
		break;

		// LOAD REG, IMMEDIATE
	case 0x06: case 0x0E: case 0x16: case 0x1E: case 0x26: case 0x2E: case 0x3E:
		dst = static_cast<Register>(opcode >> 3 & 0x7);
		// const byte& value = memory[pc + 1];
		// m_registers.

		break;
	case 0x3F: // CCF
		m_registers.complement_carry_flag();
		m_registers.increment_clock_cycles(4, 1);
		break;
		// LOAD REG, REG
	case 0x40:	case 0x41:	case 0x42:	case 0x43:	case 0x44:	case 0x45:	case 0x47:	// LD B, <reg>
	case 0x48:	case 0x49:	case 0x4A:	case 0x4B:	case 0x4C:	case 0x4D:	case 0x4F:	// LD C, <reg>
	case 0x50:	case 0x51:	case 0x52:	case 0x53:	case 0x54:	case 0x55:	case 0x57:	// LD D, <reg>
	case 0x58:	case 0x59:	case 0x5A:	case 0x5B:	case 0x5C:	case 0x5D:	case 0x5F:	// LD E, <reg>
	case 0x60:	case 0x61:	case 0x62:	case 0x63:	case 0x64:	case 0x65:	case 0x67:	// LD H, <reg>
	case 0x68:	case 0x69:	case 0x6A:	case 0x6B:	case 0x6C:	case 0x6D:	case 0x6F:	// LD L, <reg>
	case 0x78:	case 0x79:	case 0x7A:	case 0x7B:	case 0x7C:	case 0x7D:	case 0x7F:	// LD A, <reg>
		dst = static_cast<Register>(opcode >> 3 & 0x7);
		src = m_registers.get_register(static_cast<Register>(opcode & 0x7));
		m_registers.set_register(dst, src);
		m_registers.increment_clock_cycles(4, 1);
		break;
	case 0x80:	case 0x81:	case 0x82:	case 0x83:	case 0x84:	case 0x85:	case 0x87:	// ADD A, <reg>
	case 0x88:	case 0x89:	case 0x8A:	case 0x8B:	case 0x8C:	case 0x8D:	case 0x8F:	// ADC A, <reg>
		carry = opcode & 0x8; // if true, handle carry
		dst = static_cast<Register>(opcode >> 3 & 0x7);
		src = m_registers.get_register(static_cast<Register>(opcode & 0x7));
		m_registers.register_addition(dst, src, carry);
		m_registers.increment_clock_cycles(4, 1);
		break;
	case 0x90:	case 0x91:	case 0x92:	case 0x93:	case 0x94:	case 0x95:	case 0x97:	// SUB A, <reg>
	case 0x98:	case 0x99:	case 0x9A:	case 0x9B:	case 0x9C:	case 0x9D:	case 0x9F:	// SBC A, <reg>
		carry = opcode & 0x8; // if true, handle carry
		dst = static_cast<Register>(opcode >> 3 & 0x7);
		src = m_registers.get_register(static_cast<Register>(opcode & 0x7));
		m_registers.register_subtraction(dst, src);
		m_registers.increment_clock_cycles(4, 1);
		break;
	case 0xA0:	case 0xA1:	case 0xA2:	case 0xA3:	case 0xA4:	case 0xA5:	case 0xA7:	// AND A, <reg>
		// handle flags
		dst = static_cast<Register>(opcode >> 3 & 0x7);
		src = m_registers.get_register(static_cast<Register>(opcode & 0x7));
		m_registers.register_bitwise_and(dst, src);
		m_registers.increment_clock_cycles(4, 1);
		break;
	case 0xA8:	case 0xA9:	case 0xAA:	case 0xAB:	case 0xAC:	case 0xAD:	case 0xAF:	// XOR A, <reg>
		// handle flags
		dst = static_cast<Register>(opcode >> 3 & 0x7);
		src = m_registers.get_register(static_cast<Register>(opcode & 0x7));
		m_registers.register_bitwise_xor(dst, src);
		m_registers.increment_clock_cycles(4, 1);
		break;
	case 0xB0:	case 0xB1:	case 0xB2:	case 0xB3:	case 0xB4:	case 0xB5:	case 0xB7:	// OR A, <reg>
		// handle flags
		dst = static_cast<Register>(opcode >> 3 & 0x7);
		src = m_registers.get_register(static_cast<Register>(opcode & 0x7));
		m_registers.register_bitwise_or(dst, src);
		m_registers.increment_clock_cycles(4, 1);
		break;
		// CP A, <reg>
	case 0xB8:	case 0xB9:	case 0xBA:	case 0xBB:	case 0xBC:	case 0xBD:	case 0xBF:	// missing 0xBE
		break;
	case 0xC0: break;
	case 0xD0: break;
	case 0xE0: break;
	case 0xF0: break;

	case 0xD3: case 0xDB: case 0xDD:
	case 0xE3: case 0xE4: case 0xEB: case 0xEC: case 0xED:
	case 0xF4: case 0xFC: case 0xFD:
	default:
		fprintf(stderr, "Invalid opcode!");
		exit(1);
	}
}

word CPU::decode()
{
	return 0x00;
}