#include "pemsa/memory/pemsa_memory_module.hpp"
#include "pemsa/pemsa_emulator.hpp"

#include "sdl/sdl_graphics_backend.hpp"

static SDL_Color palette[] = {
	{ 0, 0, 0, 255 },
	{ 29, 43, 83, 255 },
	{ 126, 37, 83, 255 },
	{ 0, 135, 81, 255 },
	{ 171, 82, 54, 255 },
	{ 95, 87, 79, 255 },
	{ 194, 195, 199, 255 },
	{ 255, 241, 232, 255 },
	{ 255, 0, 77, 255 },
	{ 255, 163, 0, 255 },
	{ 255, 236, 39, 255 },
	{ 0, 228, 54, 255 },
	{ 41, 173, 255, 255 },
	{ 131, 118, 156, 255 },
	{ 255, 119, 168, 255 },
	{ 255, 204, 170, 255 }
};

#define UNPACK_COLOR(i)	(palette[i].r << 24) + (palette[i].g << 16) + (palette[i].b << 8) + 255

static int intPalette[] = {
	UNPACK_COLOR(0), UNPACK_COLOR(1), UNPACK_COLOR(2), UNPACK_COLOR(3),
	UNPACK_COLOR(4), UNPACK_COLOR(5), UNPACK_COLOR(6), UNPACK_COLOR(7),
	UNPACK_COLOR(8), UNPACK_COLOR(9), UNPACK_COLOR(10), UNPACK_COLOR(11),
	UNPACK_COLOR(12), UNPACK_COLOR(13), UNPACK_COLOR(14), UNPACK_COLOR(15)
};

#undef UNPACK_COLOR

SdlGraphicsBackend::SdlGraphicsBackend(SDL_Window *window) {
	this->window = window;
	this->resize();
}

SdlGraphicsBackend::~SdlGraphicsBackend() {
	SDL_FreeSurface(this->surface);
}

void SdlGraphicsBackend::createSurface() {
	this->surface = SDL_CreateRGBSurfaceWithFormat(0, 128, 128, 32, SDL_PIXELFORMAT_RGBA8888);
}

void SdlGraphicsBackend::flip() {
	Uint32* pixels = (Uint32*) this->surface->pixels;
	uint8_t* ram = this->emulator->getMemoryModule()->ram;
	PemsaDrawStateModule* drawStateModule = this->emulator->getDrawStateModule();

	int screenPalette[16];

	for (int i = 0; i < 16; i++) {
		screenPalette[i] = drawStateModule->getScreenColor(i);
	}

	for (int i = 0; i < 0x2000; i++) {
		uint8_t val = ram[i + PEMSA_RAM_SCREEN];

		// TODO: screen colors
		pixels[i * 2] = intPalette[screenPalette[val & 0x0f]];
		pixels[i * 2 + 1] = intPalette[screenPalette[val >> 4]];
	}
}

SDL_Surface *SdlGraphicsBackend::getSurface() {
	return this->surface;
}

float SdlGraphicsBackend::getScale() {
	return this->scale;
}

int SdlGraphicsBackend::getOffsetX() {
	return this->offsetX;
}

int SdlGraphicsBackend::getOffsetY() {
	return this->offsetY;
}

void SdlGraphicsBackend::handleEvent(SDL_Event *event) {
	if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED) {
		SDL_FillRect(SDL_GetWindowSurface(this->window), NULL, 0x000000);
		this->resize();
	}
}

void SdlGraphicsBackend::resize() {
	int width, height;
	SDL_GetWindowSize(this->window, &width, &height);

	this->scale = floor(fmin(width / 128, height / 128));
	this->offsetX = (width - this->scale * 128) / 2;
	this->offsetY = (height - this->scale * 128) / 2;
}