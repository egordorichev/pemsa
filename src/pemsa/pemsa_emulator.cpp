#include "pemsa/pemsa_emulator.hpp"

PemsaEmulator::PemsaEmulator(PemsaGraphicsBackend *graphics, PemsaAudioBackend *audio, PemsaInputBackend *input) {
	graphics->setEmulator(this);
	audio->setEmulator(this);
	input->setEmulator(this);

	this->modules[0] = (PemsaModule*) (this->graphics = new PemsaGraphicsModule(this, graphics));
	this->modules[1] = (PemsaModule*) (this->audio = new PemsaAudioModule(this, audio));
	this->modules[2] = (PemsaModule*) (this->input = new PemsaInputModule(this, input));
}

PemsaEmulator::~PemsaEmulator() {
	for (int i = 0; i < PEMSA_MODULE_COUNT; i++) {
		delete this->modules[i];
	}
}
