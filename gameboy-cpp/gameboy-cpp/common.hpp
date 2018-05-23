#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>

typedef uint8_t byte;
typedef int8_t ibyte;
typedef uint16_t word;
typedef int16_t iword;

enum Register
{
	R_B,
	R_C,
	R_D,
	R_E,
	R_H,
	R_L,
	R_F,
	R_A,
};

enum CombinedRegister
{
	R_BC,
	R_DE,
	R_HL,
	R_AF,
};

namespace Contants
{
	const int REGISTER_COUNT = 8;
}