#include "pemsa/memory/pemsa_draw_state_module.hpp"
#include "pemsa/pemsa_emulator.hpp"

PemsaDrawStateModule::PemsaDrawStateModule(PemsaEmulator *emulator) : PemsaModule(emulator) {
	this->reset();
}

void PemsaDrawStateModule::reset() {
	uint8_t* ram = this->emulator->getMemoryModule()->ram;

	ram[PEMSA_RAM_PALETTE0] = 0x10;
	ram[PEMSA_RAM_PALETTE1] = 0x0;
	ram[PEMSA_RAM_CURSOR_X] = 0;
	ram[PEMSA_RAM_CURSOR_Y] = 0;
	ram[PEMSA_RAM_CAMERA_X] = 0;
	ram[PEMSA_RAM_CAMERA_Y] = 0;

	for (int i = 1; i < 16; i++) {
		ram[PEMSA_RAM_PALETTE0 + i] = (uint8_t) i;
		ram[PEMSA_RAM_PALETTE1 + i] = (uint8_t) i;
	}

	ram[PEMSA_RAM_CLIP_LEFT] = 0;
	ram[PEMSA_RAM_CLIP_TOP] = 0;
	ram[PEMSA_RAM_CLIP_RIGHT] = 128;
	ram[PEMSA_RAM_CLIP_BOTTOM] = 128;

	ram[PEMSA_RAM_DRAW_COLOR] = 6;
}

bool PemsaDrawStateModule::isTransparent(int color) {
	return this->emulator->getMemoryModule()->ram[PEMSA_RAM_PALETTE0 + color] > 15;
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
	if (replacement < 0 || replacement > 143 || (replacement > 15 && replacement < 128)) {
		return;
	}

	this->emulator->getMemoryModule()->ram[PEMSA_RAM_PALETTE1 + (color & 0x0f)] = replacement;
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
	*((int16_t*) (emulator->getMemoryModule()->ram + PEMSA_RAM_FILL_PATTERN)) = p;
}

int PemsaDrawStateModule::getFillPattern() {
	return *((int16_t*) (emulator->getMemoryModule()->ram + PEMSA_RAM_FILL_PATTERN));
}

void PemsaDrawStateModule::setFillPatternTransparent(bool transparent) {
	emulator->getMemoryModule()->ram[PEMSA_RAM_FILL_PATTERN + 2] = transparent ? 1 : 0;
}

bool PemsaDrawStateModule::isFillPatternTransparent() {
	return emulator->getMemoryModule()->ram[PEMSA_RAM_FILL_PATTERN + 2] != 0;
}

bool PemsaDrawStateModule::getFillPatternBit(int x, int y) {
	x %= 4;
	y %= 4;

	int i = y * 4 + x;
	return ((this->getFillPattern() & (1 << 15) >> i) >> (15 - i)) != 0;
}

int PemsaDrawStateModule::getCameraX() {
	return *((int16_t*) (emulator->getMemoryModule()->ram + PEMSA_RAM_CAMERA_X));
}

void PemsaDrawStateModule::setCameraX(int x) {
	*((int16_t*) (emulator->getMemoryModule()->ram + PEMSA_RAM_CAMERA_X)) = x;
}

int PemsaDrawStateModule::getCameraY() {
	return *((int16_t*) (emulator->getMemoryModule()->ram + PEMSA_RAM_CAMERA_Y));
}

void PemsaDrawStateModule::setCameraY(int y) {
	*((int16_t*) (emulator->getMemoryModule()->ram + PEMSA_RAM_CAMERA_Y)) = y;
}

int PemsaDrawStateModule::getCursorX() {
	return emulator->getMemoryModule()->ram[PEMSA_RAM_CURSOR_X];
}

void PemsaDrawStateModule::setCursorX(int x) {
	emulator->getMemoryModule()->ram[PEMSA_RAM_CURSOR_X] = x;
}

int PemsaDrawStateModule::getCursorY() {
	return emulator->getMemoryModule()->ram[PEMSA_RAM_CURSOR_Y];
}

void PemsaDrawStateModule::setCursorY(int y) {
	emulator->getMemoryModule()->ram[PEMSA_RAM_CURSOR_Y] = y;
}