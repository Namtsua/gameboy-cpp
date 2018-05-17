#pragma once
#include "common.hpp"
#include "mmu.hpp"

using namespace std;

class Cartridge
{
public:
	Cartridge(MMU* mmu);
	~Cartridge();
	bool load(const string& rom);
	MMU* m_mmu;
};