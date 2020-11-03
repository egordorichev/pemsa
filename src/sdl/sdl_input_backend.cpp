#include "sdl/sdl_input_backend.hpp"
#include "sdl/sdl_graphics_backend.hpp"

#include "pemsa/util/pemsa_util.hpp"
#include "pemsa/pemsa_emulator.hpp"

#include <iostream>

static int scancode_to_button(SDL_Scancode code) {
	switch (code) {
		case SDL_SCANCODE_LEFT: case SDL_SCANCODE_A: return 0;
		case SDL_SCANCODE_RIGHT: case SDL_SCANCODE_D: return 1;
		case SDL_SCANCODE_UP: case SDL_SCANCODE_W: return 2;
		case SDL_SCANCODE_DOWN: case SDL_SCANCODE_S: return 3;
		case SDL_SCANCODE_C: case SDL_SCANCODE_Z: return 4;
		case SDL_SCANCODE_X: return 5;
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

		case SDL_MOUSEBUTTONDOWN: {
			if (event->button.button < 4) {
				this->mouseState = SET_BIT(this->mouseState, event->button.button, true);
			}

			break;
		}

		case SDL_MOUSEBUTTONUP: {
			if (event->button.button < 4) {
				this->mouseState = SET_BIT(this->mouseState, event->button.button, false);
			}

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

	return this->state[p][i] == 1;
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

int SdlInputBackend::getMouseX() {
	int x, y;
	SDL_GetMouseState(&x, &y);

	SdlGraphicsBackend* graphics = (SdlGraphicsBackend*) this->emulator->getGraphicsModule()->getBackend();
	return (x - graphics->getOffsetX()) / graphics->getScale();
}

int SdlInputBackend::getMouseY() {
	int x, y;
	SDL_GetMouseState(&x, &y);

	SdlGraphicsBackend* graphics = (SdlGraphicsBackend*) this->emulator->getGraphicsModule()->getBackend();
	return (y - graphics->getOffsetY()) / graphics->getScale();
}

int SdlInputBackend::getMouseMask() {
	return this->mouseState;
}
