#include "pemsa/memory/pemsa_memory_module.hpp"
#include "pemsa/util/pemsa_util.hpp"
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

uint8_t PemsaMemoryModule::getPixel(int x, int y, int region) {
	int index = (y * 128 + x) / 2;

	if (index < 0 || index > 64 * 128 - 1) {
		return 0x10;
	}

	return GET_HALF(ram[index + region], (x & 1) == 0);
}