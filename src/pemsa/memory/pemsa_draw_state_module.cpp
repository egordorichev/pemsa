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
	ram[PEMSA_RAM_CLIP_RIGHT] = 128;
	ram[PEMSA_RAM_CLIP_BOTTOM] = 128;
}

bool PemsaDrawStateModule::isTransparent(int color) {
	return (this->emulator->getMemoryModule()->ram[PEMSA_RAM_PALETTE0 + color] & 0x10) != 0;
}

void PemsaDrawStateModule::setTransparent(int color, bool transparent) {
	uint8_t* ram = emulator->getMemoryModule()->ram;
	color &= 0x0f;

	if (transparent) {
		ram[PEMSA_RAM_PALETTE0 + color] &= 0x0f;
		ram[PEMSA_RAM_PALETTE0 + color] |= 0x10;
	} else {
		ram[PEMSA_RAM_PALETTE0 + color] &= 0x0f;
	}
}

int PemsaDrawStateModule::getScreenColor(int color) {
	return this->emulator->getMemoryModule()->ram[PEMSA_RAM_PALETTE1 + (color & 0x0f)];
}

void PemsaDrawStateModule::setScreenColor(int color, int replacement) {
	this->emulator->getMemoryModule()->ram[PEMSA_RAM_PALETTE1 + (color & 0x0f)] = replacement & 0x0f;
}

int PemsaDrawStateModule::getDrawColor(int color) {
	return this->emulator->getMemoryModule()->ram[PEMSA_RAM_PALETTE0 + (color & 0x0f)];
}

void PemsaDrawStateModule::setDrawColor(int color, int replacement) {
	bool wasTransparent = this->isTransparent(color);

	this->emulator->getMemoryModule()->ram[PEMSA_RAM_PALETTE0 + (color & 0x0f)] = replacement & 0x0f;
	this->setTransparent(color, wasTransparent);
}

int PemsaDrawStateModule::getColor() {
	return this->emulator->getMemoryModule()->ram[PEMSA_RAM_DRAW_COLOR];
}

void PemsaDrawStateModule::setColor(int color) {
	this->emulator->getMemoryModule()->ram[PEMSA_RAM_DRAW_COLOR] = color & 0x0f;
}

void PemsaDrawStateModule::setFillPattern(int p) {
	uint8_t* ram = emulator->getMemoryModule()->ram;

	ram[PEMSA_RAM_FILL_PATTERN] = p & 0xff;
	ram[PEMSA_RAM_FILL_PATTERN + 1] = (p >> 8) & 0xff;
}

int PemsaDrawStateModule::getFillPattern() {
	uint8_t* ram = emulator->getMemoryModule()->ram;
	return ram[PEMSA_RAM_FILL_PATTERN] | (ram[PEMSA_RAM_FILL_PATTERN + 1] << 8);
}

void PemsaDrawStateModule::setFillPatternTransparent(bool transparent) {
	emulator->getMemoryModule()->ram[PEMSA_RAM_FILL_PATTERN + 2] = transparent ? 1 : 0;
}

bool PemsaDrawStateModule::isFillPatternTransparent() {
	return emulator->getMemoryModule()->ram[PEMSA_RAM_FILL_PATTERN + 2] != 0;
}

int PemsaDrawStateModule::getFillPatternBit(int x, int y) {
	x &= 0b11;
	y &= 0b11;

	int i = (y << 2) + x;
	return (this->getFillPattern() & (1 << 15) >> i) >> (15 - i);
}
