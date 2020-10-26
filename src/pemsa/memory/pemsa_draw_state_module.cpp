#include "pemsa/memory/pemsa_draw_state_module.hpp"
#include "pemsa/pemsa_emulator.hpp"

PemsaDrawStateModule::PemsaDrawStateModule(PemsaEmulator *emulator) : PemsaModule(emulator) {
	this->reset();
}

void PemsaDrawStateModule::reset() {
	uint8_t* ram = this->emulator->getMemoryModule()->ram;

	ram[PEMSA_RAM_PALETTE0] = 0x10;
	ram[PEMSA_RAM_PALETTE1] = 0x0;

	for (int i = 1; i < 16; i++) {
		ram[PEMSA_RAM_PALETTE0 + i] = (uint8_t) i;
		ram[PEMSA_RAM_PALETTE1 + i] = (uint8_t) i;
	}

	ram[PEMSA_RAM_CLIP_LEFT] = 0;
	ram[PEMSA_RAM_CLIP_TOP] = 0;
	ram[PEMSA_RAM_CLIP_RIGHT] = 127;
	ram[PEMSA_RAM_CLIP_BOTTOM] = 127;
}

bool PemsaDrawStateModule::isTransparent(int color) {
	return (this->emulator->getMemoryModule()->ram[PEMSA_RAM_PALETTE0 + color] & 0x10) != 0;
}
