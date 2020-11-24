#ifndef PEMSA_MEMORY_MODULE_HPP
#define PEMSA_MEMORY_MODULE_HPP

#include "pemsa/pemsa_module.hpp"
#include <cstdint>

#define PEMSA_RAM_GFX 0x0
#define PEMSA_RAM_GFX_MAP 0x1000
#define PEMSA_RAM_MAP 0x2000
#define PEMSA_RAM_GFX_PROPS 0x3000
#define PEMSA_RAM_SONG 0x3100
#define PEMSA_RAM_SFX 0x3200
#define PEMSA_RAM_USER 0x4300
#define PEMSA_RAM_CART 0x5e00
#define PEMSA_RAM_PALETTE0 0x5f00
#define PEMSA_RAM_PALETTE1 0x5f10
#define PEMSA_RAM_CLIP_LEFT 0x5f20
#define PEMSA_RAM_CLIP_TOP 0x5f21
#define PEMSA_RAM_CLIP_RIGHT 0x5f22
#define PEMSA_RAM_CLIP_BOTTOM 0x5f23
#define PEMSA_RAM_DRAW_COLOR 0x5f25
#define PEMSA_RAM_CURSOR_X 0x5f26
#define PEMSA_RAM_CURSOR_Y 0x5f27
#define PEMSA_RAM_CAMERA_X 0x5f28
#define PEMSA_RAM_CAMERA_Y 0x5f2a
#define PEMSA_RAM_FILL_PATTERN 0x5f31
#define PEMSA_RAM_DRAW_MODE 0x5f2c
#define PEMSA_RAM_LINE_X 0x5f3c
#define PEMSA_RAM_LINE_Y 0x5f3e
#define PEMSA_RAM_SCREEN_X 0x5F2C
#define PEMSA_RAM_SCREEN_Y 0x5F2C
#define PEMSA_RAM_SCREEN 0x6000
#define PEMSA_RAM_END 0x8000

class PemsaMemoryModule : public PemsaModule {
	public:
		PemsaMemoryModule(PemsaEmulator* emulator);
		void reset() override;

		void setPixel(int x, int y, int c, int region);
		uint8_t getPixel(int x, int y, int region);

		// Don't kill me for using a public field, PERFORMANCE
		uint8_t ram[PEMSA_RAM_END];
};

void pemsa_open_memory_api(PemsaEmulator* machine, lua_State* state);

#endif