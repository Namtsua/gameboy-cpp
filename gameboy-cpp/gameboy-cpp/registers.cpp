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
	AF.upper_register = &A;
	AF.lower_register = &F;
	BC.upper_register = &B;
	BC.lower_register = &C;
	DE.upper_register = &D;
	DE.lower_register = &E;
	HL.upper_register = &H;
	HL.lower_register = &L;
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

void Registers::register_subtraction(const Register &r, const byte& value, const bool& carry /* = false */)
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

void Registers::increment_stack_pointer()
{
	++SP;
}

void Registers::decrement_stack_pointer()
{
	--SP;
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