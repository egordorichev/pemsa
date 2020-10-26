#include "sdl/sdl_input_backend.hpp"
#include <iostream>

static int scancode_to_button(SDL_Scancode code) {
	switch (code) {
		case SDL_SCANCODE_LEFT: case SDL_SCANCODE_A: return 0;
		case SDL_SCANCODE_RIGHT: case SDL_SCANCODE_D: return 1;
		case SDL_SCANCODE_UP: case SDL_SCANCODE_W: return 2;
		case SDL_SCANCODE_DOWN: case SDL_SCANCODE_S: return 3;
		case SDL_SCANCODE_X: return 4;
		case SDL_SCANCODE_C: case SDL_SCANCODE_Z: return 5;
	}

	return -1;
}

void SdlInputBackend::handleEvent(SDL_Event *event) {
	switch (event->type) {
		case SDL_KEYDOWN: {
			int button = scancode_to_button(event->key.keysym.scancode);

			if (button != -1 && this->state[0][button] == 0) {
				this->state[0][button] = 1;
			}

			break;
		}

		case SDL_KEYUP: {
			int button = scancode_to_button(event->key.keysym.scancode);

			if (button != -1) {
				this->state[0][button] = 0;
			}

			break;
		}

		case SDL_CONTROLLERBUTTONDOWN: {
			// TODO
			break;
		}

		case SDL_CONTROLLERBUTTONUP: {
			// TODO
			break;
		}
	}
}

bool SdlInputBackend::isButtonDown(int i, int p) {
	if (i < 0 || i > PEMSA_BUTTON_COUNT - 1 || p < 0 || p > PEMSA_PLAYER_COUNT - 1) {
		return false;
	}

	return this->state[p][i] >= 1;
}

bool SdlInputBackend::isButtonPressed(int i, int p) {
	if (i < 0 || i > PEMSA_BUTTON_COUNT - 1 || p < 0 || p > PEMSA_PLAYER_COUNT - 1) {
		return false;
	}

	return this->state[p][i] == 2;
}

void SdlInputBackend::update() {
	for (int p = 0; p < PEMSA_PLAYER_COUNT; p++) {
		uint8_t* buttons = this->state[p];

		for (int i = 0; i < PEMSA_BUTTON_COUNT; i++) {
			uint8_t value = buttons[i];

			if (value == 0) {
				continue;
			}

			buttons[i] = (value) % 15 + 1;
		}
	}
}