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

SdlGraphicsBackend::SdlGraphicsBackend(SDL_Window *window) {
	this->window = window;
}

SdlGraphicsBackend::~SdlGraphicsBackend() {
	SDL_FreeSurface(this->surface);
}

void SdlGraphicsBackend::createSurface() {
	this->surface = SDL_CreateRGBSurfaceWithFormat(0, 128, 128, 32, SDL_PIXELFORMAT_RGBA8888);
}

void SdlGraphicsBackend::flip() {
	Uint32* pixels = (Uint32*) this->surface->pixels;

	SDL_Color color = palette[(int) (SDL_GetTicks() / 1000) % 16];
	int c = (color.r << 24) + (color.g << 16) + (color.b << 8) + 255;

	for (int i = 0; i < 128 * 128; i++) {
		pixels[i] = c;
	}
}

SDL_Surface *SdlGraphicsBackend::getSurface() {
	return this->surface;
}
