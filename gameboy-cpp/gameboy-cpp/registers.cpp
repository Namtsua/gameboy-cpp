#include "registers.hpp"

using namespace Contants;

void Registers::initialize()
{
	for (int i = 0; i < REGISTER_COUNT; ++i)
		set_register(static_cast<Register>(i), 0x0); // Reset all registers

	set_stack_pointer(0x0); // Reset stack pointer
	set_program_counter(0x0); // Reset program counter
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

void Registers::register_addition(const Register &r, const byte& value)
{
	switch (r)
	{
		case R_A: A += value; break;
		case R_B: B += value; break;
		case R_C: C += value; break;
		case R_D: D += value; break;
		case R_E: E += value; break;
		case R_F: F += value; break;
		case R_H: H += value; break;
		case R_L: L += value; break;
		default:
			fprintf(stderr, "Unknown register specified");
			exit(1);
	}
}

void Registers::register_subtraction(const Register &r, const byte& value)
{
	switch (r)
	{
		case R_A: A -= value; break;
		case R_B: B -= value; break;
		case R_C: C -= value; break;
		case R_D: D -= value; break;
		case R_E: E -= value; break;
		case R_F: F -= value; break;
		case R_H: H -= value; break;
		case R_L: L -= value; break;
		default:
			fprintf(stderr, "Unknown register specified");
			exit(1);
	}
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
	switch (r)
	{
		case R_A: A &= value; break;
		case R_B: B &= value; break;
		case R_C: C &= value; break;
		case R_D: D &= value; break;
		case R_E: E &= value; break;
		case R_F: F &= value; break;
		case R_H: H &= value; break;
		case R_L: L &= value; break;
		default:
			fprintf(stderr, "Unknown register specified");
			exit(1);
	}
}

void Registers::register_bitwise_or(const Register &r, const byte& value)
{
	switch (r)
	{
		case R_A: A |= value; break;
		case R_B: B |= value; break;
		case R_C: C |= value; break;
		case R_D: D |= value; break;
		case R_E: E |= value; break;
		case R_F: F |= value; break;
		case R_H: H |= value; break;
		case R_L: L |= value; break;
		default:
			fprintf(stderr, "Unknown register specified");
			exit(1);
	}
}

void Registers::register_bitwise_xor(const Register &r, const byte& value)
{
	switch (r)
	{
		case R_A: A ^= value; break;
		case R_B: B ^= value; break;
		case R_C: C ^= value; break;
		case R_D: D ^= value; break;
		case R_E: E ^= value; break;
		case R_F: F ^= value; break;
		case R_H: H ^= value; break;
		case R_L: L ^= value; break;
		default:
			fprintf(stderr, "Unknown register specified");
			exit(1);
	}
}

void Registers::increment_register(const Register& r)
{
	switch (r)
	{
		case R_A: ++A;	break;
		case R_B: ++B;	break;
		case R_C: ++C;	break;
		case R_D: ++D;	break;
		case R_E: ++E;	break;
		case R_F: ++F;	break;
		case R_H: ++H;	break;
		case R_L: ++L;	break;
		default:
			fprintf(stderr, "Unknown register specified");
			exit(1);
	}
}
void Registers::decrement_register(const Register& r)
{
	switch (r)
	{
		case R_A: --A;	break;
		case R_B: --B;	break;
		case R_C: --C;	break;
		case R_D: --D;	break;
		case R_E: --E;	break;
		case R_F: --F;	break;
		case R_H: --H;	break;
		case R_L: --L;	break;
		default:
			fprintf(stderr, "Unknown register specified");
			exit(1);
	}
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

void Registers::increment_machine_cycle(const byte& value)
{
	m += value;
}

void Registers::increment_tick_cycle(const byte& value)
{
	t += value;
}