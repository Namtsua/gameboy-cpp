#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef uint8_t byte;
typedef uint16_t word;

enum Register
{
	R_A,
	R_B,
	R_C,
	R_D,
	R_E,
	R_F,
	R_H,
	R_L
};