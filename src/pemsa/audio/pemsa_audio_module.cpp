#include "pemsa/audio/pemsa_audio_module.hpp"

PemsaAudioModule::PemsaAudioModule(PemsaEmulator* emulator, PemsaAudioBackend* backend) : PemsaModule(emulator) {
	this->backend = backend;
}

PemsaAudioModule::~PemsaAudioModule() {
	delete this->backend;
}