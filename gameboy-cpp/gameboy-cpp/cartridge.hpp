#pragma once
#include "common.hpp"

using namespace std;

class Cartridge
{
	Cartridge(const string& file_path);
	~Cartridge();
	bool load();

private:
	const char* rom_path;
};