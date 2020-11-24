#include "pemsa/input/pemsa_input_module.hpp"

PemsaInputModule::PemsaInputModule(PemsaEmulator* emulator, PemsaInputBackend* backend) : PemsaModule(emulator) {
	this->backend = backend;
}

PemsaInputModule::~PemsaInputModule() {
	delete this->backend;
}

bool PemsaInputModule::isDown(int button, int player) {
	return this->backend->isButtonDown(button, player);
}

bool PemsaInputModule::isPressed(int button, int player) {
	return this->backend->isButtonPressed(button, player);
}

PemsaInputBackend *PemsaInputModule::getBackend() {
	return this->backend;
}

void PemsaInputModule::updateInput() {
	this->backend->update();
}

void PemsaInputModule::reset() {
	this->backend->reset();
}