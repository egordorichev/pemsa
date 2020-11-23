#include "pemsa/memory/pemsa_memory_module.hpp"
#include "pemsa/pemsa_emulator.hpp"

#include "sdl/sdl_graphics_backend.hpp"

#include <iostream>

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
	{ 255, 204, 170, 255 },

	{ 41, 24, 2 },
	{ 17, 29, 53 },
	{ 66, 33, 54 },
	{ 18, 83, 89 },
	{ 116, 47, 41 },
	{ 73, 51, 59 },
	{ 162, 136, 121 },
	{ 243, 239, 125 },
	{ 190, 18, 80 },
	{ 255, 108, 36 },
	{ 168, 231, 46 },
	{ 0, 181, 67 },
	{ 6, 90, 181 },
	{ 117, 70, 101 },
	{ 255, 110, 89 },
	{ 255, 157, 129 }
};

#define UNPACK_COLOR(i)	(palette[i].r << 24) + (palette[i].g << 16) + (palette[i].b << 8) + 255

static int intPalette[] = {
	UNPACK_COLOR(0), UNPACK_COLOR(1), UNPACK_COLOR(2), UNPACK_COLOR(3),
	UNPACK_COLOR(4), UNPACK_COLOR(5), UNPACK_COLOR(6), UNPACK_COLOR(7),
	UNPACK_COLOR(8), UNPACK_COLOR(9), UNPACK_COLOR(10), UNPACK_COLOR(11),
	UNPACK_COLOR(12), UNPACK_COLOR(13), UNPACK_COLOR(14), UNPACK_COLOR(15),

	UNPACK_COLOR(16), UNPACK_COLOR(17), UNPACK_COLOR(18), UNPACK_COLOR(19),
	UNPACK_COLOR(20), UNPACK_COLOR(21), UNPACK_COLOR(22), UNPACK_COLOR(23),
	UNPACK_COLOR(24), UNPACK_COLOR(25), UNPACK_COLOR(26), UNPACK_COLOR(27),
	UNPACK_COLOR(28), UNPACK_COLOR(29), UNPACK_COLOR(30), UNPACK_COLOR(31)
};

#undef UNPACK_COLOR

SdlGraphicsBackend::SdlGraphicsBackend(SDL_Window *window) {
	this->window = window;
	this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (this->renderer == nullptr) {
		std::cerr << "Failed to create renderer: " << SDL_GetError() << "\n";
	}

	this->resize();
}

SdlGraphicsBackend::~SdlGraphicsBackend() {
	SDL_FreeSurface(this->surface);
	SDL_DestroyRenderer(this->renderer);
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
		int c = drawStateModule->getScreenColor(i);

		if (c > 128) {
			c -= 128 - 16;
		}

		screenPalette[i] = c;
	}

	for (int i = 0; i < 0x2000; i++) {
		uint8_t val = ram[i + PEMSA_RAM_SCREEN];

		pixels[i * 2] = intPalette[screenPalette[val & 0x0f]];
		pixels[i * 2 + 1] = intPalette[screenPalette[val >> 4]];
	}

	this->emulator->getInputModule()->updateInput();
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

void SdlGraphicsBackend::render() {
	SDL_Rect src = { 0, 0, 128, 128 };
	SDL_Rect dst = { this->getOffsetX(), this->getOffsetY(), (int) (this->getScale() * 128), (int) (this->getScale() * 128) };

	int angle = 0;
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	SDL_Texture* texture = SDL_CreateTextureFromSurface(this->renderer, this->surface);
	SDL_RenderClear(this->renderer);

	PemsaDrawMode drawMode = this->emulator->getGraphicsModule()->getDrawMode();

	if (drawMode != SCREEN_NORMAL) {
		switch (drawMode) {
			case SCREEN_HORIZONTAL_STRETCH: {
				src.w = 64;
				break;
			}

			case SCREEN_VERTICAL_STRETCH: {
				src.h = 64;
				break;
			}

			case SCREEN_STRETCH: {
				src.w = 64;
				src.h = 64;
				break;
			}

			case SCREEN_HORIZONTAL_MIRROR: {
				src.w = 64;

				dst.w /= 2;
				dst.x += dst.w;

				SDL_RenderCopyEx(this->renderer, texture, &src, &dst, 0, NULL, SDL_FLIP_HORIZONTAL);

				dst.x -= dst.w;
				break;
			}

			case SCREEN_VERTICAL_MIRROR: {
				src.h = 64;

				dst.h /= 2;
				dst.y += dst.h;

				SDL_RenderCopyEx(this->renderer, texture, &src, &dst, 0, NULL, SDL_FLIP_VERTICAL);

				dst.y -= dst.h;
				break;
			}

			case SCREEN_MIRROR: {
				src.w = 64;
				src.h = 64;

				dst.w /= 2;
				dst.h /= 2;
				dst.x += dst.w;

				SDL_RenderCopyEx(this->renderer, texture, &src, &dst, 0, NULL, SDL_FLIP_HORIZONTAL);
				dst.y += dst.h;

				SDL_RenderCopyEx(this->renderer, texture, &src, &dst, 0, NULL,
				                 (SDL_RendererFlip) (SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL));
				dst.x -= dst.w;

				SDL_RenderCopyEx(this->renderer, texture, &src, &dst, 0, NULL, SDL_FLIP_VERTICAL);
				dst.y -= dst.h;

				break;
			}

			case SCREEN_HORIZONTAL_FLIP: {
				flip = SDL_FLIP_HORIZONTAL;
				break;
			}

			case SCREEN_VERTICAL_FLIP: {
				flip = SDL_FLIP_VERTICAL;
				break;
			}

			case SCREEN_90_ROTATION: {
				angle = 90;
				break;
			}

			case SCREEN_180_ROTATION: {
				angle = 180;
				break;
			}

			case SCREEN_270_ROTATION: {
				angle = 270;
				break;
			}
		}
	}

	SDL_RenderCopyEx(this->renderer, texture, &src, &dst, angle, NULL, flip);

	SDL_RenderPresent(this->renderer);
	SDL_DestroyTexture(texture);
}

void SdlGraphicsBackend::setFps(int fps) {
	this->fps = fps;
}

int SdlGraphicsBackend::getFps() {
	return this->fps;
}