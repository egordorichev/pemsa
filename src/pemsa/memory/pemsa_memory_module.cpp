#include "pemsa/memory/pemsa_memory_module.hpp"
#include "pemsa/util/pemsa_util.hpp"
#include <cstring>

PemsaMemoryModule::PemsaMemoryModule(PemsaEmulator *emulator) : PemsaModule(emulator) {
	this->reset();
}

void PemsaMemoryModule::setPixel(int x, int y, int c, int region) {
	if (x < ram[PEMSA_RAM_CLIP_LEFT] || y < ram[PEMSA_RAM_CLIP_TOP] || x >= ram[PEMSA_RAM_CLIP_RIGHT] || y >= ram[PEMSA_RAM_CLIP_BOTTOM]) {
		return;
	}

	int index = (((y << 7) + x) >> 1) + region;
	ram[index] = SET_HALF(ram[index], c, (x & 1) == 0);
}

uint8_t PemsaMemoryModule::getPixel(int x, int y, int region) {
	if (x < 0 || y < 0 || x > 127 || y > 127) {
		return 0;
	}

	return GET_HALF(ram[(y * 128 + x) / 2 + region], (x & 1) == 0);
}

void PemsaMemoryModule::reset() {
	memset(this->ram, 0, PEMSA_RAM_END);
}