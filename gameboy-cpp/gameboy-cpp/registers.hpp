#pragma once
#include "common.hpp"

#define ZERO_FLAG 0x80
#define SUB_FLAG 0x40
#define HALF_CARRY_FLAG 0x20
#define CARRY_FLAG 0x10
#define CLEAR_FLAG(flag) F &= ~(flag)
#define GET_FLAG(flag) F & flag
#define SET_FLAG(flag) F |= flag

class Registers
{
public:
	void initialize();
	void set_register(const Register& r, const byte& value);
	byte get_register(const Register& r) const;
	void register_addition(const Register &r, const byte& value, const bool& carry = false);
	void register_subtraction(const Register &r, const byte& value, const bool& carry = false);
	void register_multiplication(const Register &r, const byte& value);
	void register_bitwise_and(const Register &r, const byte& value);
	void register_bitwise_or(const Register &r, const byte& value);
	void register_bitwise_xor(const Register &r, const byte& value);
	void register_compare(const Register &r, const byte& value);
	word get_stack_pointer() const;
	void set_stack_pointer(const word& value);
	void increment_stack_pointer();
	void decrement_stack_pointer();
	void complement_register_A();
	void complement_carry_flag();
	void set_carry_flag();
	word get_program_counter() const;
	void set_program_counter(const word& value);
	void increment_register(const Register& r);
	void decrement_register(const Register& r);
	void increment_clock_cycles(const byte& t_val, const byte& m_val);

	/*inline void set_zero_flag(const byte& reg)
	{
		if (!reg) F |= ZERO_FLAG;
		else CLEAR_FLAG(ZERO_FLAG);
	}

	inline void set_sub_flag()
	{
		F |= SUB_FLAG;
	}

	inline void set_half_carry_flag(const byte& reg_value, const byte& value, const byte& carry)
	{
		if ((int)(reg_value & 0x0F) + (int)(value & 0x0F) + (int)carry > 0x0F) SET_FLAG(HALF_CARRY_FLAG);
		else CLEAR_FLAG(HALF_CARRY_FLAG);
	}

	inline void set_carry_flag(const byte& reg_value, const byte& value, const byte& carry)
	{
		if ((int)(reg_value & 0xFF) + (int)(value & 0xFF) + (int)carry > 0xFF) F |= CARRY_FLAG;
		else CLEAR_FLAG(CARRY_FLAG);
	}*/

	struct combined_register
	{
		byte* upper_register;
		byte* lower_register;
	};


private:
	byte A; byte F;
	byte B; byte C;
	byte D; byte E;
	byte H; byte L;
	word SP;
	word PC;
	byte m;
	byte t;
	combined_register AF;
	combined_register BC;
	combined_register DE;
	combined_register HL;

};

