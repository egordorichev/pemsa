cmake_minimum_required(VERSION 3.16)
project(pemsa)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(libs/fixmath)
add_subdirectory(libs/lua)

add_library(pemsa STATIC src/pemsa/pemsa_emulator.cpp src/pemsa/pemsa_backend.cpp src/pemsa/pemsa_module.cpp
	src/pemsa/audio/pemsa_audio_module.cpp src/pemsa/graphics/pemsa_graphics_module.cpp src/pemsa/input/pemsa_input_module.cpp
	src/pemsa/cart/pemsa_cartridge_module.cpp src/pemsa/memory/pemsa_memory_module.cpp src/pemsa/graphics/pemsa_graphics_api.cpp
	src/pemsa/util/pemsa_system_api.cpp src/pemsa/util/pemsa_math_api.cpp src/pemsa/util/pemsa_util.cpp src/pemsa/input/pemsa_input_api.cpp
	src/pemsa/memory/pemsa_draw_state_module.cpp src/pemsa/memory/pemsa_draw_state_api.cpp src/pemsa/memory/pemsa_memory_api.cpp
	src/pemsa/util/pemsa_font.cpp src/pemsa/cart/pemsa_scanner.cpp src/pemsa/cart/pemsa_emitter.cpp
	src/pemsa/cart/pemsa_cartridge_api.cpp src/pemsa/audio/pemsa_audio_api.cpp src/pemsa/audio/pemsa_wave_functions.cpp
	src/pemsa/audio/pemsa_audio_channel.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/lua/ ${CMAKE_CURRENT_SOURCE_DIR}/libs/fixmath/ ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_link_libraries(pemsa customlua fixmath stdc++fs)
set_target_properties(pemsa PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dist/)