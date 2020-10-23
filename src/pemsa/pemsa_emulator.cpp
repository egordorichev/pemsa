#include "pemsa/pemsa_emulator.hpp"

PemsaEmulator::PemsaEmulator(PemsaGraphicsBackend *graphics, PemsaAudioBackend *audio, PemsaInputBackend *input) {
	graphics->setEmulator(this);
	audio->setEmulator(this);
	input->setEmulator(this);

	this->modules[0] = (PemsaModule*) (this->graphicsModule = new PemsaGraphicsModule(this, graphics));
	this->modules[1] = (PemsaModule*) (this->audioModule = new PemsaAudioModule(this, audio));
	this->modules[2] = (PemsaModule*) (this->inputModule = new PemsaInputModule(this, input));
	this->modules[3] = (PemsaModule*) (this->cartridgeModule = new PemsaCartridgeModule(this));
}

PemsaEmulator::~PemsaEmulator() {
	for (int i = 0; i < PEMSA_MODULE_COUNT; i++) {
		delete this->modules[i];
	}
}

void PemsaEmulator::update(double dt) {
	for (int i = 0; i < PEMSA_MODULE_COUNT; i++) {
		this->modules[i]->update();
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
