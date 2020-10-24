#include "pemsa/memory/pemsa_memory_module.hpp"
#include <cstring>

PemsaMemoryModule::PemsaMemoryModule(PemsaEmulator *emulator) : PemsaModule(emulator) {
	memset(this->ram, 0, PEMSA_RAM_END);
}

void PemsaMemoryModule::setPixel(int x, int y, int c, int region) {
	int index = (((y << 7) + x) >> 1) + region;

	// todo: count clip

	if ((x & 1) == 0) {
		ram[index] = (uint8_t) ((uint8_t) (ram[index] & 0xf0) | (c & 0x0f));
	} else {
		ram[index] = (uint8_t) ((uint8_t) (ram[index] & 0x0f) | ((c & 0x0f) << 4));
	}
}