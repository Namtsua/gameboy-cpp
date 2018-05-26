#include "registers.hpp"

using namespace Contants;

void Registers::initialize()
{
	for (int i = 0; i < REGISTER_COUNT; ++i)
		set_register(static_cast<Register>(i), 0x0); // Reset all registers

	//set_stack_pointer(0xFFFE); // Reset stack pointer
	set_stack_pointer(0x00);
	set_program_counter(0x00); // Reset program counter
	m = 0; // Reset machine cycle clock
	t = 0; // Reset tick cycle clock
}

void Registers::set_register(const Register& r, const byte& value)
{
	switch (r)
	{
	case R_A: A = value; break;
	case R_B: B = value; break;
	case R_C: C = value; break;
	case R_D: D = value; break;
	case R_E: E = value; break;
	case R_F: F = value; break;
	case R_H: H = value; break;
	case R_L: L = value; break;
	default:
		fprintf(stderr, "Unknown register specified");
		break;
	}
}

void Registers::set_register(const CombinedRegister& r, const word& value)
{
	switch (r)
	{
	case R_BC:	B = value & 0xFF00; C = value & 0x00FF; break;
	case R_DE:	D = value & 0xFF00; C = value & 0x00FF; break;
	case R_HL:	H = value & 0xFF00; L = value & 0x00FF; break;
	case R_AF:	A = value & 0xFF00; F = value & 0x00FF; break;
	default:
		fprintf(stderr, "Unknown combined register specified");
		exit(1);
	}
}

// check logic
void Registers::load_HL_with_SP(const iword& value)
{
	word result = SP + value;
	word carry = (SP ^ value ^ result) & 0xFFFF;
	CLEAR_FLAG(ZERO_FLAG);
	CLEAR_FLAG(SUB_FLAG);

	if ((carry & 0x100) == 0x100)
		SET_FLAG(CARRY_FLAG);
	else
		CLEAR_FLAG(CARRY_FLAG);

	if ((carry & 0x10) == 0x10)
		SET_FLAG(HALF_CARRY_FLAG);
	else
		CLEAR_FLAG(HALF_CARRY_FLAG);

	set_register(R_HL, result);
}

byte Registers::get_register(const Register& r) const
{
	switch (r)
	{
	case R_A: return A;
	case R_B: return B;
	case R_C: return C;
	case R_D: return D;
	case R_E: return E;
	case R_F: return F;
	case R_H: return H;
	case R_L: return L;
	default:
		fprintf(stderr, "Unknown register specified");
		exit(1);
	}
}

word Registers::get_register(const CombinedRegister& r) const
{
	switch (r)
	{
	case R_BC:	return get_register(R_B) << 8 | get_register(R_C);
	case R_DE:	return get_register(R_D) << 8 | get_register(R_E);
	case R_HL:	return get_register(R_H) << 8 | get_register(R_L);
	case R_AF:	return get_register(R_A) << 8 | get_register(R_F);
	default:
		fprintf(stderr, "Unknown combined register specified");
		exit(1);
	}
}


void Registers::register_addition(const Register &r, const byte& value, const bool& carry /*=false*/)
{
	const byte& reg_value = get_register(r);
	byte result = reg_value + value;
	CLEAR_FLAG(SUB_FLAG);

	if (carry)
	{
		const byte& carry = GET_FLAG(CARRY_FLAG) ? 1 : 0;
		result += carry;

		if ((int)(reg_value & 0x0F) + (int)(value & 0x0F) + (int)carry > 0x0F)
			SET_FLAG(HALF_CARRY_FLAG);
		else
			CLEAR_FLAG(HALF_CARRY_FLAG);

		if ((int)(reg_value & 0xFF) + (int)(value & 0xFF) + (int)carry > 0xFF)
			SET_FLAG(CARRY_FLAG);
		else
			CLEAR_FLAG(CARRY_FLAG);
	}
	else
	{
		if ((int)(reg_value & 0x0F) + (int)(value & 0x0F) > 0x0F)
			SET_FLAG(HALF_CARRY_FLAG);
		else
			CLEAR_FLAG(HALF_CARRY_FLAG);

		if ((int)(reg_value & 0xFF) + (int)(value & 0xFF) > 0xFF)
			SET_FLAG(CARRY_FLAG);
		else
			CLEAR_FLAG(CARRY_FLAG);
	}

	if (result == 0x00)
		SET_FLAG(ZERO_FLAG);
	else
		CLEAR_FLAG(ZERO_FLAG);

	set_register(r, result);
}

// check flag logic
void Registers::register_addition(const CombinedRegister& r, const word& value)
{
	const word& reg_value = get_register(r);
	word result = reg_value + value;
	CLEAR_FLAG(SUB_FLAG);

	if (result < reg_value)
		SET_FLAG(CARRY_FLAG);
	else
		CLEAR_FLAG(CARRY_FLAG);

	if ((result ^ reg_value ^ value) & 0x1000)
		SET_FLAG(HALF_CARRY_FLAG);
	else
		CLEAR_FLAG(HALF_CARRY_FLAG);

	set_register(r, result);
}

void Registers::register_subtraction(const Register& r, const byte& value, const bool& carry /* = false */)
{
	const byte& reg_value = get_register(r);
	byte result = reg_value - value;
	SET_FLAG(SUB_FLAG);

	if (carry)
	{
		const byte& carry = GET_FLAG(CARRY_FLAG) ? 1 : 0;
		result += carry;

		if ((int)(reg_value & 0x0F) < ((int)(value & 0x0F) + (int)carry))
			SET_FLAG(HALF_CARRY_FLAG);
		else
			CLEAR_FLAG(HALF_CARRY_FLAG);

		if ((int)(reg_value & 0xFF) < ((int)(value & 0x0F) + (int)carry))
			SET_FLAG(CARRY_FLAG);
		else
			CLEAR_FLAG(CARRY_FLAG);
	}
	else
	{
		if ((int)(reg_value & 0x0F) < (int)(value & 0x0F))
			SET_FLAG(HALF_CARRY_FLAG);
		else
			CLEAR_FLAG(CARRY_FLAG);

		if ((int)(reg_value & 0xFF) < (int)(value & 0xFF))
			SET_FLAG(CARRY_FLAG);
		else
			CLEAR_FLAG(CARRY_FLAG);
	}

	if (result == 0x00)
		SET_FLAG(ZERO_FLAG);
	else
		CLEAR_FLAG(ZERO_FLAG);
	set_register(r, result);
}

void Registers::register_multiplication(const Register &r, const byte& value)
{
	switch (r)
	{
	case R_A: A *= value; break;
	case R_B: B *= value; break;
	case R_C: C *= value; break;
	case R_D: D *= value; break;
	case R_E: E *= value; break;
	case R_F: F *= value; break;
	case R_H: H *= value; break;
	case R_L: L *= value; break;
	default:
		fprintf(stderr, "Unknown register specified");
		exit(1);
	}
}

void Registers::register_bitwise_and(const Register &r, const byte& value)
{
	const byte& reg_value = get_register(r);
	byte result = reg_value & value;

	if (result == 0x00)
		SET_FLAG(ZERO_FLAG);
	else
		CLEAR_FLAG(ZERO_FLAG);

	CLEAR_FLAG(SUB_FLAG);
	CLEAR_FLAG(CARRY_FLAG);
	SET_FLAG(HALF_CARRY_FLAG);
	set_register(r, result);
}

void Registers::register_bitwise_or(const Register &r, const byte& value)
{
	const byte& reg_value = get_register(r);
	byte result = reg_value | value;

	if (result == 0x00)
		SET_FLAG(ZERO_FLAG);
	else
		CLEAR_FLAG(ZERO_FLAG);

	CLEAR_FLAG(SUB_FLAG);
	CLEAR_FLAG(HALF_CARRY_FLAG);
	CLEAR_FLAG(CARRY_FLAG);
	set_register(r, result);
}

void Registers::register_bitwise_xor(const Register &r, const byte& value)
{
	const byte& reg_value = get_register(r);
	byte result = reg_value ^ value;

	if (result == 0x00)
		SET_FLAG(ZERO_FLAG);
	else
		CLEAR_FLAG(ZERO_FLAG);

	CLEAR_FLAG(SUB_FLAG);
	CLEAR_FLAG(HALF_CARRY_FLAG);
	CLEAR_FLAG(CARRY_FLAG);
	set_register(r, result);
}

void Registers::register_compare(const Register &r, const byte& value)
{
	const byte& reg_value = get_register(r);
	byte result = reg_value - value;

	if (result == 0x00)
		SET_FLAG(ZERO_FLAG);
	else
		CLEAR_FLAG(ZERO_FLAG);

	SET_FLAG(SUB_FLAG);

	if ((reg_value & 0x0F) - (value & 0x0F) < 0)
		SET_FLAG(HALF_CARRY_FLAG);
	else
		SET_FLAG(HALF_CARRY_FLAG);

	if (reg_value < value)
		SET_FLAG(CARRY_FLAG);
	else
		CLEAR_FLAG(CARRY_FLAG);
}

void Registers::register_rotate_left(const Register& r, const byte& value)
{
	byte result = value;
	const byte& carry = value & 0x80 ? 1 : 0;
	result <<= 1;
	
	if (carry)
	{
		SET_FLAG(CARRY_FLAG);
		result |= 0x1;
	}
	else
		CLEAR_FLAG(CARRY_FLAG);

	if (result == 0x00)
		SET_FLAG(ZERO_FLAG);
	else
		CLEAR_FLAG(ZERO_FLAG);

	CLEAR_FLAG(SUB_FLAG);
	CLEAR_FLAG(HALF_CARRY_FLAG);
	set_register(r, result);
}

void Registers::register_rotate_left_carry(const Register& r, const byte& value)
{
	byte result = value;
	const byte& old_carry = GET_FLAG(CARRY_FLAG);
	const byte& carry = value & 0x80 ? SET_FLAG(CARRY_FLAG) : CLEAR_FLAG(CARRY_FLAG);
	
	// Shift left
	result <<= 1;
	
	// Replace lowest bit by old carry value
	if (old_carry) result |= 0x1;

	if (result == 0x00)
		SET_FLAG(ZERO_FLAG);
	else
		CLEAR_FLAG(ZERO_FLAG);

	CLEAR_FLAG(SUB_FLAG);
	CLEAR_FLAG(HALF_CARRY_FLAG);
	set_register(r, result);
}

void Registers::register_rotate_right(const Register& r, const byte& value)
{
	byte result = value;
	const byte& carry = value & 0x1 ? 1 : 0;
	result >>= 1;

	if (carry)
	{
		SET_FLAG(CARRY_FLAG);
		result |= 0x80;
	}
	else
		CLEAR_FLAG(CARRY_FLAG);

	if (result == 0x00)
		SET_FLAG(ZERO_FLAG);
	else
		CLEAR_FLAG(ZERO_FLAG);

	CLEAR_FLAG(SUB_FLAG);
	CLEAR_FLAG(HALF_CARRY_FLAG);
	set_register(r, result);
}

void Registers::register_rotate_right_carry(const Register& r, const byte& value)
{

	byte result = value;
	const byte& old_carry = GET_FLAG(CARRY_FLAG);
	const byte& carry = value & 0x1 ? SET_FLAG(CARRY_FLAG) : CLEAR_FLAG(CARRY_FLAG);

	// Shift right
	result >>= 1;

	// Replace highest bit by old carry value
	if (old_carry) result |= 0x80;

	if (carry)
	{
		SET_FLAG(CARRY_FLAG);
		result |= 0x80;
	}
	else
		CLEAR_FLAG(CARRY_FLAG);

	if (result == 0x00)
		SET_FLAG(ZERO_FLAG);
	else
		CLEAR_FLAG(ZERO_FLAG);

	CLEAR_FLAG(SUB_FLAG);
	CLEAR_FLAG(HALF_CARRY_FLAG);
	set_register(r, result);
}

void Registers::register_shift_left(const Register& r, const byte& value)
{
	byte result = value;
	const byte& carry = value & 0x80 ? SET_FLAG(CARRY_FLAG) : CLEAR_FLAG(CARRY_FLAG);

	// Shift left and set LSB to 0
	result <<= 1;
	result &= 0x0;

	if (result == 0x00)
		SET_FLAG(ZERO_FLAG);
	else
		CLEAR_FLAG(ZERO_FLAG);

	CLEAR_FLAG(SUB_FLAG);
	CLEAR_FLAG(HALF_CARRY_FLAG);
	set_register(r, result);
}


void Registers::register_shift_right(const Register& r, const byte& value)
{
	byte result = value;
	const byte& carry = value & 0x1 ? SET_FLAG(CARRY_FLAG) : CLEAR_FLAG(CARRY_FLAG);

	result >>= 1;

	if (result == 0x00)
		SET_FLAG(ZERO_FLAG);
	else
		CLEAR_FLAG(ZERO_FLAG);

	CLEAR_FLAG(SUB_FLAG);
	CLEAR_FLAG(HALF_CARRY_FLAG);
	set_register(r, result);
}

void Registers::register_shift_right_carry(const Register& r, const byte& value)
{
	byte result = value;
	const byte& carry = value & 0x1 ? SET_FLAG(CARRY_FLAG) : CLEAR_FLAG(CARRY_FLAG);

	result = (result >> 1) | (result & 0x80);

	if (result == 0x00)
		SET_FLAG(ZERO_FLAG);
	else
		CLEAR_FLAG(ZERO_FLAG);

	CLEAR_FLAG(SUB_FLAG);
	CLEAR_FLAG(HALF_CARRY_FLAG);
	set_register(r, result);
}

void Registers::swap(const Register& r, const byte& value)
{
	byte result = (value & 0xF << 4) | (value & 0xF0 >> 4);
	CLEAR_FLAG(SUB_FLAG);
	CLEAR_FLAG(CARRY_FLAG);
	CLEAR_FLAG(HALF_CARRY_FLAG);

	if (result == 0x00)
		SET_FLAG(ZERO_FLAG);
	else
		CLEAR_FLAG(ZERO_FLAG);

	set_register(r, result);
}

void Registers::bit(const Register& r, const byte& bit)
{
	byte result = (get_register(r) >> bit) & 0x1;
	if (result)
		SET_FLAG(ZERO_FLAG);
	else
		CLEAR_FLAG(ZERO_FLAG);

	CLEAR_FLAG(SUB_FLAG);
	SET_FLAG(HALF_CARRY_FLAG);
}

void Registers::increment_register(const Register& r)
{
	byte result = get_register(r) + 1;
	if ((result & 0x0F) == 0x00)
		SET_FLAG(HALF_CARRY_FLAG);
	else
		CLEAR_FLAG(HALF_CARRY_FLAG);

	if (result == 0x00)
		SET_FLAG(ZERO_FLAG);
	else
		CLEAR_FLAG(ZERO_FLAG);

	CLEAR_FLAG(SUB_FLAG);
	set_register(r, result);
}


void Registers::increment_register(const CombinedRegister& r)
{
	word result = get_register(r);
	set_register(r, result + 1);
}

void Registers::decrement_register(const Register& r)
{
	byte result = get_register(r) - 1;

	if (result = 0x00)
		SET_FLAG(ZERO_FLAG);
	else
		CLEAR_FLAG(ZERO_FLAG);

	if ((result & 0x0F) == 0x0F)
		SET_FLAG(HALF_CARRY_FLAG);
	else
		CLEAR_FLAG(HALF_CARRY_FLAG);

	SET_FLAG(SUB_FLAG);
	set_register(r, result);
}

void Registers::decrement_register(const CombinedRegister& r)
{
	word result = get_register(r);
	set_register(r, result - 1);
}

void Registers::complement_register_A()
{
	const byte& result = ~A;
	set_register(R_A, result);
}

void Registers::complement_carry_flag()
{
	CLEAR_FLAG(SUB_FLAG);
	CLEAR_FLAG(HALF_CARRY_FLAG);
	SET_FLAG(~GET_FLAG(CARRY_FLAG));
}

void Registers::set_carry_flag()
{
	CLEAR_FLAG(SUB_FLAG);
	CLEAR_FLAG(HALF_CARRY_FLAG);
	SET_FLAG(CARRY_FLAG);
}

word Registers::get_stack_pointer() const
{
	return SP;
}

void Registers::set_stack_pointer(const word& value)
{
	SP = value;
}

void Registers::stack_pointer_signed_addition(const ibyte& value)
{
	byte result = SP + value;

	if ((result & 0xF) < (SP & 0xF))
		SET_FLAG(HALF_CARRY_FLAG);
	else
		CLEAR_FLAG(HALF_CARRY_FLAG);

	if ((result & 0xFF) < (SP & 0xFF))
		SET_FLAG(CARRY_FLAG);
	else
		CLEAR_FLAG(CARRY_FLAG);

	CLEAR_FLAG(ZERO_FLAG);
	CLEAR_FLAG(SUB_FLAG);
}

void Registers::increment_stack_pointer()
{
	++SP;
}

void Registers::decrement_stack_pointer()
{
	--SP;
}

bool Registers::get_flag(const byte& flag) const
{
	return (GET_FLAG(flag)) != 0x0;
}

word Registers::get_program_counter() const
{
	return PC;
}

void Registers::set_program_counter(const word& value)
{
	PC = value;
}

void Registers::advance_program_counter(const word& value)
{
	PC += value;
}

void Registers::increment_clock_cycles(const byte& t_val, const byte& m_val)
{
	t += t_val;
	m += m_val;
}