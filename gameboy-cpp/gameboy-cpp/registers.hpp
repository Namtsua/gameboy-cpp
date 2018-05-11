#pragma once
#include "common.hpp"

class Registers
{
	public:
		void initialize();
		void set_register(const Register& r, const byte& value);
		byte get_register(const Register& r) const;
		void register_addition(const Register &r, const byte& value);
		void register_subtraction(const Register &r, const byte& value);
		void register_multiplication(const Register &r, const byte& value);
		void register_bitwise_and(const Register &r, const byte& value);
		void register_bitwise_or(const Register &r, const byte& value);
		void register_bitwise_xor(const Register &r, const byte& value);
		word get_stack_pointer() const;
		void set_stack_pointer(const word& value);
		void increment_stack_pointer();
		void decrement_stack_pointer();
		word get_program_counter() const;
		void set_program_counter(const word& value);
		void increment_register(const Register& r);
		void decrement_register(const Register& r);
		void increment_machine_cycle(const byte& value);
		void increment_tick_cycle(const byte& value);

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