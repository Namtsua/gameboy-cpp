#pragma once
#include "registers.hpp"

class CPU
{
public:
	CPU();
	~CPU();
	void cycle();
	word decode();

	// 0x0X opcodes
	void execute_0x_opcode();
	void execute_00_opcode();
	void execute_01_opcode();
	void execute_02_opcode();
	void execute_03_opcode();
	void execute_04_opcode();
	void execute_05_opcode();
	void execute_06_opcode();
	void execute_07_opcode();
	void execute_08_opcode();
	void execute_09_opcode();
	void execute_0A_opcode();
	void execute_0B_opcode();
	void execute_0C_opcode();
	void execute_0D_opcode();
	void execute_0E_opcode();
	void execute_0F_opcode();

	// 0x1X opcodes
	void execute_1x_opcode();
	void execute_10_opcode();
	void execute_11_opcode();
	void execute_12_opcode();
	void execute_13_opcode();
	void execute_14_opcode();
	void execute_15_opcode();
	void execute_16_opcode();
	void execute_17_opcode();
	void execute_18_opcode();
	void execute_19_opcode();
	void execute_1A_opcode();
	void execute_1B_opcode();
	void execute_1C_opcode();
	void execute_1D_opcode();
	void execute_1E_opcode();
	void execute_1F_opcode();

	// 0x2X opcodes
	void execute_2x_opcode();
	void execute_20_opcode();
	void execute_21_opcode();
	void execute_22_opcode();
	void execute_23_opcode();
	void execute_24_opcode();
	void execute_25_opcode();
	void execute_26_opcode();
	void execute_27_opcode();
	void execute_28_opcode();
	void execute_29_opcode();
	void execute_2A_opcode();
	void execute_2B_opcode();
	void execute_2C_opcode();
	void execute_2D_opcode();
	void execute_2E_opcode();
	void execute_2F_opcode();

	// 0x3X opcodes
	void execute_3x_opcode();
	void execute_30_opcode();
	void execute_31_opcode();
	void execute_32_opcode();
	void execute_33_opcode();
	void execute_34_opcode();
	void execute_35_opcode();
	void execute_36_opcode();
	void execute_37_opcode();
	void execute_38_opcode();
	void execute_39_opcode();
	void execute_3A_opcode();
	void execute_3B_opcode();
	void execute_3C_opcode();
	void execute_3D_opcode();
	void execute_3E_opcode();
	void execute_3F_opcode();

	// 0x4X opcodes
	void execute_4x_opcode();
	void execute_40_opcode();
	void execute_41_opcode();
	void execute_42_opcode();
	void execute_43_opcode();
	void execute_44_opcode();
	void execute_45_opcode();
	void execute_46_opcode();
	void execute_47_opcode();
	void execute_48_opcode();
	void execute_49_opcode();
	void execute_4A_opcode();
	void execute_4B_opcode();
	void execute_4C_opcode();
	void execute_4D_opcode();
	void execute_4E_opcode();
	void execute_4F_opcode();

	// 0x5X opcodes
	void execute_5x_opcode();
	void execute_50_opcode();
	void execute_51_opcode();
	void execute_52_opcode();
	void execute_53_opcode();
	void execute_54_opcode();
	void execute_55_opcode();
	void execute_56_opcode();
	void execute_57_opcode();
	void execute_58_opcode();
	void execute_59_opcode();
	void execute_5A_opcode();
	void execute_5B_opcode();
	void execute_5C_opcode();
	void execute_5D_opcode();
	void execute_5E_opcode();
	void execute_5F_opcode();

	// 0x6X opcodes
	void execute_6x_opcode();
	void execute_60_opcode();
	void execute_61_opcode();
	void execute_62_opcode();
	void execute_63_opcode();
	void execute_64_opcode();
	void execute_65_opcode();
	void execute_66_opcode();
	void execute_67_opcode();
	void execute_68_opcode();
	void execute_69_opcode();
	void execute_6A_opcode();
	void execute_6B_opcode();
	void execute_6C_opcode();
	void execute_6D_opcode();
	void execute_6E_opcode();
	void execute_6F_opcode();

	// 0x7X opcodes
	void execute_7x_opcode();
	void execute_70_opcode();
	void execute_71_opcode();
	void execute_72_opcode();
	void execute_73_opcode();
	void execute_74_opcode();
	void execute_75_opcode();
	void execute_76_opcode();
	void execute_77_opcode();
	void execute_78_opcode();
	void execute_79_opcode();
	void execute_7A_opcode();
	void execute_7B_opcode();
	void execute_7C_opcode();
	void execute_7D_opcode();
	void execute_7E_opcode();
	void execute_7F_opcode();

	// 0x8X opcodes
	void execute_8x_opcode();
	void execute_80_opcode();
	void execute_81_opcode();
	void execute_82_opcode();
	void execute_83_opcode();
	void execute_84_opcode();
	void execute_85_opcode();
	void execute_86_opcode();
	void execute_87_opcode();
	void execute_88_opcode();
	void execute_89_opcode();
	void execute_8A_opcode();
	void execute_8B_opcode();
	void execute_8C_opcode();
	void execute_8D_opcode();
	void execute_8E_opcode();
	void execute_8F_opcode();

	// 0x9X opcodes
	void execute_9x_opcode();
	void execute_90_opcode();
	void execute_91_opcode();
	void execute_92_opcode();
	void execute_93_opcode();
	void execute_94_opcode();
	void execute_95_opcode();
	void execute_96_opcode();
	void execute_97_opcode();
	void execute_98_opcode();
	void execute_99_opcode();
	void execute_9A_opcode();
	void execute_9B_opcode();
	void execute_9C_opcode();
	void execute_9D_opcode();
	void execute_9E_opcode();
	void execute_9F_opcode();

	// 0xAX opcodes
	void execute_Ax_opcode();
	void execute_A0_opcode();
	void execute_A1_opcode();
	void execute_A2_opcode();
	void execute_A3_opcode();
	void execute_A4_opcode();
	void execute_A5_opcode();
	void execute_A6_opcode();
	void execute_A7_opcode();
	void execute_A8_opcode();
	void execute_A9_opcode();
	void execute_AA_opcode();
	void execute_AB_opcode();
	void execute_AC_opcode();
	void execute_AD_opcode();
	void execute_AE_opcode();
	void execute_AF_opcode();

	// 0xBX opcodes
	void execute_Bx_opcode();
	void execute_B0_opcode();
	void execute_B1_opcode();
	void execute_B2_opcode();
	void execute_B3_opcode();
	void execute_B4_opcode();
	void execute_B5_opcode();
	void execute_B6_opcode();
	void execute_B7_opcode();
	void execute_B8_opcode();
	void execute_B9_opcode();
	void execute_BA_opcode();
	void execute_BB_opcode();
	void execute_BC_opcode();
	void execute_BD_opcode();
	void execute_BE_opcode();
	void execute_BF_opcode();

	// 0xCX opcodes
	void execute_Cx_opcode();
	void execute_C0_opcode();
	void execute_C1_opcode();
	void execute_C2_opcode();
	void execute_C3_opcode();
	void execute_C4_opcode();
	void execute_C5_opcode();
	void execute_C6_opcode();
	void execute_C7_opcode();
	void execute_C8_opcode();
	void execute_C9_opcode();
	void execute_CA_opcode();
	void execute_CB_opcode();
	void execute_CC_opcode();
	void execute_CD_opcode();
	void execute_CE_opcode();
	void execute_CF_opcode();

	// 0xDX opcodes
	void execute_Dx_opcode();
	void execute_D0_opcode();
	void execute_D1_opcode();
	void execute_D2_opcode();
	void execute_D3_opcode();
	void execute_D4_opcode();
	void execute_D5_opcode();
	void execute_D6_opcode();
	void execute_D7_opcode();
	void execute_D8_opcode();
	void execute_D9_opcode();
	void execute_DA_opcode();
	void execute_DB_opcode();
	void execute_DC_opcode();
	void execute_DD_opcode();
	void execute_DE_opcode();
	void execute_DF_opcode();

	// 0xEX opcodes
	void execute_Ex_opcode();
	void execute_E0_opcode();
	void execute_E1_opcode();
	void execute_E2_opcode();
	void execute_E3_opcode();
	void execute_E4_opcode();
	void execute_E5_opcode();
	void execute_E6_opcode();
	void execute_E7_opcode();
	void execute_E8_opcode();
	void execute_E9_opcode();
	void execute_EA_opcode();
	void execute_EB_opcode();
	void execute_EC_opcode();
	void execute_ED_opcode();
	void execute_EE_opcode();
	void execute_EF_opcode();

	// 0xFX opcodes
	void execute_Fx_opcode();
	void execute_F0_opcode();
	void execute_F1_opcode();
	void execute_F2_opcode();
	void execute_F3_opcode();
	void execute_F4_opcode();
	void execute_F5_opcode();
	void execute_F6_opcode();
	void execute_F7_opcode();
	void execute_F8_opcode();
	void execute_F9_opcode();
	void execute_FA_opcode();
	void execute_FB_opcode();
	void execute_FC_opcode();
	void execute_FD_opcode();
	void execute_FE_opcode();
	void execute_FF_opcode();

private:
	Registers m_registers;
};