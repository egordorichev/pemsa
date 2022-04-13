#include "pemsa/pemsa_emulator.hpp"
#include "pemsa/util/pemsa_font.hpp"

#include <cstring>

PemsaEmulator::PemsaEmulator(PemsaGraphicsBackend *graphics, PemsaAudioBackend *audio, PemsaInputBackend *input, bool* running, bool enableSplash) {
	this->running = running;
	pemsa_setup_font();

	graphics->setEmulator(this);
	audio->setEmulator(this);
	input->setEmulator(this);

	this->modules[0] = (PemsaModule*) (this->graphicsModule = new PemsaGraphicsModule(this, graphics));
	this->modules[1] = (PemsaModule*) (this->audioModule = new PemsaAudioModule(this, audio));
	this->modules[2] = (PemsaModule*) (this->inputModule = new PemsaInputModule(this, input));
	this->modules[3] = (PemsaModule*) (this->cartridgeModule = new PemsaCartridgeModule(this, enableSplash));
	this->modules[4] = (PemsaModule*) (this->memoryModule = this->currentMemoryModule = new PemsaMemoryModule(this));
	this->modules[5] = (PemsaModule*) (this->drawStateModule = new PemsaDrawStateModule(this));
	this->modules[6] = (PemsaModule*) (this->systemMemoryModule = new PemsaMemoryModule(this));

	this->currentMemoryModule = this->systemMemoryModule;
	this->drawStateModule->reset();
	this->currentMemoryModule = this->memoryModule;

	// Clear the system screen with pink (color 15), so that it is transparent
	memset(this->systemMemoryModule->ram + PEMSA_RAM_SCREEN, (15 << 4) + 15, 0x2000);
}

PemsaEmulator::~PemsaEmulator() {
	for (int i = 0; i < PEMSA_MODULE_COUNT; i++) {
		this->modules[i]->reset();
		delete this->modules[i];
	}
}

bool PemsaEmulator::update(double dt) {
	for (int i = 0; i < PEMSA_MODULE_COUNT; i++) {
		this->modules[i]->update(dt);
	}

	return this->cartridgeModule->isDone();
}

PemsaGraphicsModule *PemsaEmulator::getGraphicsModule() {
	return this->graphicsModule;
}

PemsaAudioModule *PemsaEmulator::getAudioModule() {
	return this->audioModule;
}

PemsaInputModule *PemsaEmulator::getInputModule() {
	return this->inputModule;
}

PemsaCartridgeModule *PemsaEmulator::getCartridgeModule() {
	return this->cartridgeModule;
}

PemsaMemoryModule *PemsaEmulator::getMemoryModule() {
	return this->currentMemoryModule;
}

PemsaMemoryModule *PemsaEmulator::getActualMemoryModule() {
	return this->memoryModule;
}

PemsaMemoryModule *PemsaEmulator::getSystemMemoryModule() {
	return this->systemMemoryModule;
}

PemsaDrawStateModule *PemsaEmulator::getDrawStateModule() {
	return this->drawStateModule;
}

void PemsaEmulator::setMemoryModule(PemsaMemoryModule* module) {
	this->currentMemoryModule = module;
}

void PemsaEmulator::reset() {
	for (int i = 0; i < PEMSA_MODULE_COUNT; i++) {
		this->modules[i]->reset();
	}
}

void PemsaEmulator::stop() {
	*this->running = false;
	this->cartridgeModule->stop();
}

void PemsaEmulator::render() {
	PemsaGraphicsBackend* graphicsBackend = this->graphicsModule->getBackend();

	if (cartridgeModule->hasNewFrame()) {
		graphicsBackend->flip();
	}

	graphicsBackend->render();
}