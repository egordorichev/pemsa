cmake_minimum_required(VERSION 3.17)
project(pemsa)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/)

find_package(SDL2 REQUIRED)
find_package(Lua52)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


if (UNIX)
	set(LUA_LIBRARIES /usr/lib/x86_64-linux-gnu/liblua5.2.so)
endif (UNIX)

include_directories(include ${SDL2_INCLUDE_DIRS} ${LUA_INCLUDE_DIR}/)

add_library(pemsa STATIC src/pemsa/pemsa_emulator.cpp src/pemsa/pemsa_backend.cpp src/pemsa/pemsa_module.cpp
	src/pemsa/audio/pemsa_audio_module.cpp src/pemsa/graphics/pemsa_graphics_module.cpp src/pemsa/input/pemsa_input_module.cpp
	src/pemsa/cart/pemsa_cartridge_module.cpp src/pemsa/memory/pemsa_memory_module.cpp src/pemsa/graphics/pemsa_graphics_api.cpp
	src/pemsa/util/pemsa_system_api.cpp src/pemsa/util/pemsa_math_api.cpp src/pemsa/util/pemsa_util.cpp src/pemsa/input/pemsa_input_api.cpp
	src/pemsa/memory/pemsa_draw_state_module.cpp src/pemsa/memory/pemsa_draw_state_api.cpp src/pemsa/memory/pemsa_memory_api.cpp
	src/pemsa/util/pemsa_font.cpp src/pemsa/cart/pemsa_scanner.cpp src/pemsa/cart/pemsa_emitter.cpp
	src/pemsa/cart/pemsa_cartridge_api.cpp src/pemsa/audio/pemsa_audio_api.cpp)

target_link_libraries(pemsa ${LUA_LIBRARIES})
set_target_properties(pemsa PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dist/)

add_executable(pemsa_sdl src/sdl/main.cpp src/sdl/sdl_graphics_backend.cpp src/sdl/sdl_audio_backend.cpp src/sdl/sdl_input_backend.cpp)
target_link_libraries(pemsa_sdl pemsa ${SDL2_LIBRARIES} Threads::Threads)
set_target_properties(pemsa_sdl PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dist/)

message(${LUA52_FOUND} : ${LUA_INCLUDE_DIR} " : " ${LUA_LIBRARIES})