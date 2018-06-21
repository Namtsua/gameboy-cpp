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
	void set_register(const CombinedRegister& r, const word& value);
	void load_HL_with_SP(const iword& value);
	byte get_register(const Register& r) const;
	word get_register(const CombinedRegister& r) const;
	void register_addition(const Register& r, const byte& value, const bool& carry = false);
	void register_addition(const CombinedRegister& r, const word& value);
	void register_signed_addition(const Register&, const ibyte& value);
	void register_subtraction(const Register& r, const byte& value, const bool& carry = false);
	void register_multiplication(const Register& r, const byte& value);
	void register_bitwise_and(const Register& r, const byte& value);
	void register_bitwise_or(const Register& r, const byte& value);
	void register_bitwise_xor(const Register& r, const byte& value);
	void register_compare(const Register& r, const byte& value);
	void register_rotate_left(const Register& r, const byte& value);
	byte register_rotate_left_HL(const byte& value);
	void register_rotate_left_carry(const Register& r, const byte& value);
	byte register_rotate_left_carry_HL(const byte& value);
	void register_rotate_right(const Register& r, const byte& value);
	byte register_rotate_right_HL(const byte& value);
	void register_rotate_right_carry(const Register& r, const byte& value);
	byte register_rotate_right_carry_HL(const byte& value);
	void register_shift_left(const Register& r, const byte& value);
	byte register_shift_left_HL(const byte& value);
	void register_shift_right(const Register& r, const byte& value);
	byte register_shift_right_HL(const byte& value);
	void register_shift_right_carry(const Register& r, const byte& value);
	byte register_shift_right_carry_HL(const byte& value);
	void swap(const Register& r, const byte& value);
	byte swap_HL(const byte& value);
	void set_bit(const Register& r, const byte& bit);
	byte set_bit_HL(const byte& value, const byte& bit);
	void test_bit(const Register& r, const byte& bit);
	void test_bit_HL(const byte& value, const byte& bit);
	void reset_bit(const Register& r, const byte& bit);
	byte reset_bit_HL(const byte& value, const byte& bit);
	word get_stack_pointer() const;
	void set_stack_pointer(const word& value);
	void stack_pointer_signed_addition(const ibyte& value);
	void increment_stack_pointer();
	void decrement_stack_pointer();
	bool get_flag(const byte& flag) const;
	void complement_register_A();
	void complement_carry_flag();
	void set_carry_flag();
	word get_program_counter() const;
	void set_program_counter(const word& value);
	void advance_program_counter(const word& value);
	void increment_register(const Register& r);
	void increment_register(const CombinedRegister& r);
	void decrement_register(const Register& r);
	void decrement_register(const CombinedRegister& r);
	void increment_clock_cycles(const byte& t_val, const byte& m_val);
	std::pair<byte, byte> get_clock_cycles() const;

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
};