#include "pemsa/pemsa_emulator.hpp"
#include "pemsa/util/pemsa_font.hpp"

PemsaEmulator::PemsaEmulator(PemsaGraphicsBackend *graphics, PemsaAudioBackend *audio, PemsaInputBackend *input, bool* running) {
	this->running = running;
	pemsa_setup_font();

	graphics->setEmulator(this);
	audio->setEmulator(this);
	input->setEmulator(this);

	this->modules[0] = (PemsaModule*) (this->graphicsModule = new PemsaGraphicsModule(this, graphics));
	this->modules[1] = (PemsaModule*) (this->audioModule = new PemsaAudioModule(this, audio));
	this->modules[2] = (PemsaModule*) (this->inputModule = new PemsaInputModule(this, input));
	this->modules[3] = (PemsaModule*) (this->cartridgeModule = new PemsaCartridgeModule(this));
	this->modules[4] = (PemsaModule*) (this->memoryModule = new PemsaMemoryModule(this));
	this->modules[5] = (PemsaModule*) (this->drawStateModule = new PemsaDrawStateModule(this));
}

PemsaEmulator::~PemsaEmulator() {
	for (int i = 0; i < PEMSA_MODULE_COUNT; i++) {
		delete this->modules[i];
	}
}

void PemsaEmulator::update(double dt) {
	for (int i = 0; i < PEMSA_MODULE_COUNT; i++) {
		this->modules[i]->update(dt);
	}
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
	return this->memoryModule;
}

PemsaDrawStateModule *PemsaEmulator::getDrawStateModule() {
	return this->drawStateModule;
}

void PemsaEmulator::reset() {
	for (int i = 0; i < PEMSA_MODULE_COUNT; i++) {
		this->modules[i]->reset();
	}
}

void PemsaEmulator::stop() {
	*this->running = false;
}