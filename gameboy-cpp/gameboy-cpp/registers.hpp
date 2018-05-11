#pragma once
#include "common.hpp"

class Registers
{
	public:
		void set_register(const Register& r, const byte& value);
		byte get_register(const Register& r) const;
		word get_stack_pointer() const;
		void set_stack_pointer(const word& value);
		void increment_stack_pointer();
		void decrement_stack_pointer();
		word get_program_counter() const;
		void set_program_counter(const word& value);
		void increment_register(const Register& r);
		void decrement_register(const Register& r);

	private:
		byte A; byte F;
		byte B; byte C;
		byte D; byte E;
		byte H; byte L;
		word SP;
		word PC;
		int m;
		int t;
};