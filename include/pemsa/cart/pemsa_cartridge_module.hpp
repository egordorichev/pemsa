#ifndef PEMSA_CARTRIDGE_MODULE_HPP
#define PEMSA_CARTRIDGE_MODULE_HPP

#include "pemsa/pemsa_module.hpp"
#include "pemsa/cart/pemsa_cartridge.hpp"

#include <condition_variable>
#include <thread>
#include <future>

#define PEMSA_SAVE_CODE

class PemsaCartridgeModule : public PemsaModule {
	public:
		PemsaCartridgeModule(PemsaEmulator* emulator);
		~PemsaCartridgeModule();

		void update(double dt) override;
		bool load(const char* path, bool onlyLoad = false);
		bool save(const char* path, bool useCodeTag = true);

		void loadData(const char* path);
		void saveData();
		void stop();

		PemsaCartridge* getCart();
		std::mutex* getMutex();

		void flip();
		void setPaused(bool paused);
		void cleanupCart();
		void initiateSelfDestruct();
		void notify();
	private:
		PemsaCartridge* cart;

		std::condition_variable lock;
		std::mutex mutex;
		std::thread* gameThread;
		const char* lastLoaded;

		bool waiting;
		bool threadRunning;
		bool paused;
		bool destruct;

		void gameLoop();

		void callIfExists(const char* function);
		bool globalExists(const char* name);
		void reportLuaError();
};

void pemsa_open_cartridge_api(PemsaEmulator* machine, lua_State* state);

#endif