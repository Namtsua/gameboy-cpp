#include "cpu.hpp"

//#define LD_REG_REG(dst,src) m_registers.set_register(dst,m_registers.get_register(src)); m_registers.increment_clock_cycles(4,1)
//#define ADD_REG_REG(dst,src) m_registers.register_addition(dst, m_registers.get_register(src)); m_registers.increment_clock_cycles(4,1)
//#define SUB_REG_REG(dst,src) m_registers.register_subtraction(dst, m_registers.get_register(src)); m_registers.increment_clock_cycles(4,1)
//#define AND_REG_REG(dst,src) m_registers.register_bitwise_and(dst, m_registers.get_register(src)); m_registers.increment_clock_cycles(4,1)
//#define XOR_REG_REG(dst,src) m_registers.register_bitwise_xor(dst, m_registers.get_register(src)); m_registers.increment_clock_cycles(4,1)
//#define OR_REG_REG(dst,src) m_registers.register_bitwise_or(dst, m_registers.get_register(src)); m_registers.increment_clock_cycles(4,1)

CPU::CPU(MMU* mmu)
{
	m_registers.initialize();
	m_mmu = mmu;
}

CPU::~CPU() {}

void CPU::cycle()
{
	Register dst;
	CombinedRegister cr_dst;
	byte src;
	word cr_src;
	bool carry = false;
	word pc_step = 0x00;
	byte immediate_u8 = 0x00;
	word immediate_u16 = 0x0000;
	byte memory_value_u8;
	word memory_value_u16;
	const word& pc = m_registers.get_program_counter();
	const word& opcode = decode(pc);

	switch (opcode)
	{
		// NOP
	case 0x00:
		m_registers.increment_clock_cycles(4, 1);
		pc_step += 1;
		break;

		// LD <CReg>, <u16>
	case 0x01:	// LD BC, <u16>
	case 0x11:	// LD DE, <u16>
	case 0x21:	// LD HL, <u16>
		cr_dst = static_cast<CombinedRegister>(opcode >> 3 & 0x7);
		immediate_u16 = m_mmu->read_memory(pc + 1) << 8 | m_mmu->read_memory(pc + 2);
		m_registers.set_register(cr_dst, immediate_u16);
		m_registers.increment_clock_cycles(12, 3);
		pc_step += 3;
		break;

		// ADD HL, <CReg>
	case 0x09:	// ADD HL, BC
	case 0x19:	// ADD HL, DE
	case 0x29:	// ADD HL, HL
		cr_dst = R_HL;
		cr_src = m_registers.get_register(static_cast<CombinedRegister>(opcode & 0x7));
		//m_registers.register_addition(cr_dst, cr_src);
		m_registers.increment_clock_cycles(8, 2);
		pc_step += 2;
		break;

		// ADD HL, SP
	case 0x39:
		cr_dst = R_HL;
		cr_src = m_registers.get_program_counter();
		// m_registers.register_addition(cr_dst, cr_src);
		m_registers.increment_clock_cycles(8, 2);
		pc_step += 2;
		break;

		// INC <CReg>
	case 0x03:	// INC BC	
	case 0x13:	// INC DE
	case 0x23:	// INC HL
		cr_dst = static_cast<CombinedRegister>(opcode >> 3 & 0x7);
		m_registers.increment_register(cr_dst);
		m_registers.increment_clock_cycles(8, 2);
		pc_step += 1;
		break;

		// DEC <CReg>
	case 0x0B:	// DEC BC
	case 0x1B:	// DEC DE
	case 0x2B:	// DEC HL
		cr_dst = static_cast<CombinedRegister>(opcode >> 3 & 0x7);
		m_registers.decrement_register(cr_dst);
		m_registers.increment_clock_cycles(8, 2);
		pc_step += 1;
		break;

		// STOP
	case 0x10:	// STOP	NEEDS TO ACTUALLY STOP STUFF
		m_registers.increment_clock_cycles(4, 1);
		pc_step += 2;
		break;

	case 0x2F: // CPL
		m_registers.complement_register_A();
		m_registers.increment_clock_cycles(4, 1);
		pc_step += 1;
		break;
		//case 0x30: break;

			// INC (8-bit)
	case 0x04: case 0x0C: case 0x14: case 0x1C: case 0x24: case 0x2C: case 0x3C:
		dst = static_cast<Register>(opcode >> 3 & 0x7);
		m_registers.increment_register(dst);
		m_registers.increment_clock_cycles(4, 1);
		pc_step += 1;
		break;
			// INC (HL)
	case 0x34:
		cr_dst = R_HL;
		m_registers.increment_register(cr_dst);
		m_registers.increment_clock_cycles(12, 3);
		pc_step += 1;
		break;

			// DEC (8-bit)
	case 0x05: case 0x0D: case 0x15: case 0x1D: case 0x25: case 0x2D: case 0x3D:
		dst = static_cast<Register>(opcode >> 3 & 0x7);
		m_registers.decrement_register(dst);
		m_registers.increment_clock_cycles(4, 1);
		pc_step += 1;
		break;

			// DEC (HL)
	case 0x35:
		cr_dst = R_HL;
		m_registers.decrement_register(cr_dst);
		m_registers.increment_clock_cycles(12, 3);
		pc_step += 1;
		break;

		// LOAD REG, IMMEDIATE
	case 0x06: case 0x0E: case 0x16: case 0x1E: case 0x26: case 0x2E: case 0x3E:
		dst = static_cast<Register>(opcode >> 3 & 0x7);
		immediate_u8 = m_mmu->read_memory(pc + 1);
		m_registers.set_register(dst, immediate_u8);
		m_registers.increment_clock_cycles(8, 2);
		pc_step += 2;
		break;
	case 0x31: // LD SP, <u16>
		immediate_u16 = m_mmu->read_memory(pc + 2) << 8 | m_mmu->read_memory(pc + 1);
		m_registers.set_stack_pointer(immediate_u16);
		m_registers.increment_clock_cycles(12, 3);
		pc_step += 3;
		break;

	case 0x33:	// INC SP
		m_registers.increment_stack_pointer();
		m_registers.increment_clock_cycles(8, 2);
		pc_step += 1;
		break;

	case 0x36:	// LD (HL), u8
		immediate_u8 = m_mmu->read_memory(pc + 1);
		immediate_u16 = m_registers.get_register(R_HL);
		m_mmu->write_memory(immediate_u16, immediate_u8);
		m_registers.increment_clock_cycles(12, 3);
		pc_step += 2;

	case 0x37: // SCF
		m_registers.set_carry_flag();
		m_registers.increment_clock_cycles(4, 1);
		pc_step += 1;
		break;

	case 0x3B:	// DEC SP
		m_registers.decrement_stack_pointer();
		m_registers.increment_clock_cycles(8, 2);
		pc_step += 1;
		break;

	case 0x3F: // CCF
		m_registers.complement_carry_flag();
		m_registers.increment_clock_cycles(4, 1);
		pc_step += 1;
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
		pc_step += 1;
		break;

		// LOAD REG, (HL)
	case 0x46: case 0x4E: case 0x56: case 0x5E: case 0x66: case 0x6E: case 0x7E:
		dst = static_cast<Register>(opcode >> 3 & 0x7);
		src = R_HL;
		memory_value_u8 = m_mmu->read_memory(m_registers.get_register(R_HL));
		m_registers.set_register(dst, memory_value_u8);
		m_registers.increment_clock_cycles(8, 2);
		pc_step += 1;
		break;

		// LD (HL), <reg>
	case 0x70:	case 0x71:	case 0x72:	case 0x73:	case 0x74:	case 0x75:	case 0x77:
		immediate_u16 = m_registers.get_register(R_HL);
		src = m_registers.get_register(static_cast<Register>(opcode & 0x7));
		m_mmu->write_memory(immediate_u16, src);
		m_registers.increment_clock_cycles(8, 2);
		pc_step += 1;
		break;


	case 0x80:	case 0x81:	case 0x82:	case 0x83:	case 0x84:	case 0x85:	case 0x87:	// ADD A, <reg>
	case 0x88:	case 0x89:	case 0x8A:	case 0x8B:	case 0x8C:	case 0x8D:	case 0x8F:	// ADC A, <reg>
		carry = opcode & 0x8; // if true, handle carry
		dst = R_A;
		src = m_registers.get_register(static_cast<Register>(opcode & 0x7));
		m_registers.register_addition(dst, src, carry);
		m_registers.increment_clock_cycles(4, 1);
		pc_step += 1;
		break;

	case 0x86:	// ADD A, (HL)
	case 0x8E:	// ADC A, (HL)
		carry = opcode & 0x8;
		dst = R_A;
		src = m_mmu->read_memory(m_registers.get_register(R_HL));
		m_registers.register_addition(dst, src, carry);
		m_registers.increment_clock_cycles(8, 2);
		pc_step += 1;
		break;

	case 0x90:	case 0x91:	case 0x92:	case 0x93:	case 0x94:	case 0x95:	case 0x97:	// SUB A, <reg>
	case 0x98:	case 0x99:	case 0x9A:	case 0x9B:	case 0x9C:	case 0x9D:	case 0x9F:	// SBC A, <reg>
		carry = opcode & 0x8; // if true, handle carry
		dst = R_A;
		src = m_registers.get_register(static_cast<Register>(opcode & 0x7));
		m_registers.register_subtraction(dst, src, carry);
		m_registers.increment_clock_cycles(4, 1);
		pc_step += 1;
		break;

	case 0x96:	// SUB A, (HL)
	case 0x9E:	// SBC A, (HL)
		carry = opcode & 0x8;
		dst = R_A;
		src = m_mmu->read_memory(m_registers.get_register(R_HL));
		m_registers.register_subtraction(dst, src, carry);
		m_registers.increment_clock_cycles(8, 2);
		pc_step += 1;
		break;
	
	case 0xA0:	case 0xA1:	case 0xA2:	case 0xA3:	case 0xA4:	case 0xA5:	case 0xA7:	// AND A, <reg>
		// handle flags
		dst = R_A;
		src = m_registers.get_register(static_cast<Register>(opcode & 0x7));
		m_registers.register_bitwise_and(dst, src);
		m_registers.increment_clock_cycles(4, 1);
		pc_step += 1;
		break;

		// AND A, (HL)
	case 0xA6:
		dst = R_A;
		src = m_mmu->read_memory(m_registers.get_register(R_HL));
		m_registers.register_bitwise_and(dst, src);
		m_registers.increment_clock_cycles(8, 2);
		pc_step += 1;
		break;

	case 0xA8:	case 0xA9:	case 0xAA:	case 0xAB:	case 0xAC:	case 0xAD:	case 0xAF:	// XOR A, <reg>
		// handle flags
		dst = R_A;
		src = m_registers.get_register(static_cast<Register>(opcode & 0x7));
		m_registers.register_bitwise_xor(dst, src);
		m_registers.increment_clock_cycles(4, 1);
		pc_step += 1;
		break;

		// XOR A, (HL)
	case 0xAE:
		dst = R_A;
		src = m_mmu->read_memory(m_registers.get_register(R_HL));
		m_registers.register_bitwise_xor(dst, src);
		m_registers.increment_clock_cycles(8, 2);
		pc_step += 1;
		break;

	case 0xB0:	case 0xB1:	case 0xB2:	case 0xB3:	case 0xB4:	case 0xB5:	case 0xB7:	// OR A, <reg>
		// handle flags
		dst = R_A;
		src = m_registers.get_register(static_cast<Register>(opcode & 0x7));
		m_registers.register_bitwise_or(dst, src);
		m_registers.increment_clock_cycles(4, 1);
		pc_step += 1;
		break;

		// OR A, (HL)
	case 0xB6:
		dst = R_A;
		src = m_mmu->read_memory(m_registers.get_register(R_HL));
		m_registers.register_bitwise_or(dst, src);
		m_registers.increment_clock_cycles(8, 2);
		pc_step += 1;
		break;

	case 0xB8:	case 0xB9:	case 0xBA:	case 0xBB:	case 0xBC:	case 0xBD:	case 0xBF:	// CP A, <reg>
		dst = R_A;
		src = m_registers.get_register(static_cast<Register>(opcode & 0x7));
		m_registers.register_compare(dst, src);
		m_registers.increment_clock_cycles(4, 1);
		pc_step += 1;
		break;

		// CP A, (HL)
	case 0xBE:
		dst = R_A;
		src = m_mmu->read_memory(m_registers.get_register(R_HL));
		m_registers.register_compare(dst, src);
		m_registers.increment_clock_cycles(8, 2);
		pc_step += 1;
		break;

	case 0xC6: case 0xCE:	// ADD A, <u8> + ADC A, <u8>
		carry = opcode & 0x8; // if true, handle carry
		dst = R_A;
		immediate_u8 = m_mmu->read_memory(pc + 1);
		m_registers.register_addition(dst, immediate_u8, carry);
		m_registers.increment_clock_cycles(8, 2);
		pc_step += 2;
		break;

	case 0xD6: case 0xDE:	// SUB A, <u8> + SBC A, <u8>
		carry = opcode & 0x8; // if true, handle carry
		dst = R_A;
		immediate_u8 = m_mmu->read_memory(pc + 1);
		m_registers.register_subtraction(dst, immediate_u8, carry);
		m_registers.increment_clock_cycles(8, 2);
		pc_step += 2;
		break;

	case 0xE6:	// AND A, <u8>
		dst = R_A;
		immediate_u8 = m_mmu->read_memory(pc + 1);
		m_registers.register_bitwise_and(dst, immediate_u8);
		m_registers.increment_clock_cycles(8, 2);
		pc_step += 2;
		break;
	case 0xEE:	// XOR A, <u8>
		dst = R_A;
		immediate_u8 = m_mmu->read_memory(pc + 1);
		m_registers.register_bitwise_xor(dst, immediate_u8);
		m_registers.increment_clock_cycles(8, 2);
		pc_step += 2;
		break;
		
	case 0xF6:	// OR A, <u8>
		dst = R_A;
		immediate_u8 = m_mmu->read_memory(pc + 1);
		m_registers.register_bitwise_or(dst, immediate_u8);
		m_registers.increment_clock_cycles(8, 2);
		pc_step += 2;
		break;
	case 0xFE:	// CP A, <u8>
		dst = R_A;
		immediate_u8 = m_mmu->read_memory(pc + 1);
		m_registers.register_compare(dst, immediate_u8);
		m_registers.increment_clock_cycles(8, 2);
		pc_step += 2;
		break;

		// NOT INSTRUCTIONS
	case 0xD3: case 0xDB: case 0xDD:
	case 0xE3: case 0xE4: case 0xEB: case 0xEC: case 0xED:
	case 0xF4: case 0xFC: case 0xFD:
	default:
		fprintf(stderr, "Invalid opcode!");
		exit(1);
	}

	m_registers.advance_program_counter(pc_step);
}

word CPU::decode(const word& pc)
{
	return m_mmu->read_memory(pc);
}