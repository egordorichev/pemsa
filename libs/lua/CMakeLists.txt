cmake_minimum_required(VERSION 2.6)
project(customlua)
set(CMAKE_CXX_FLAGS_DEBUG "-g")

if (WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_library(customlua STATIC lapi.c lcode.c lctype.c ldebug.c ldo.c ldump.c lfunc.c lgc.c llex.c lmem.c lobject.c lopcodes.c lparser.c lstate.c lstring.c ltable.c ltm.c lundump.c lvm.c lzio.c lauxlib.c lbaselib.c lbitlib.c lcorolib.c ldblib.c liolib.c lmathlib.c loslib.c lstrlib.c ltablib.c loadlib.c linit.c strtofix16.c lutf8lib.c)
set_target_properties(customlua PROPERTIES INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/../fixmath/)