cmake_minimum_required(VERSION 2.6)
project(lua)

if (WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_library(lualib STATIC lapi.c lcode.c lctype.c ldebug.c ldo.c ldump.c lfunc.c lgc.c llex.c lmem.c lobject.c lopcodes.c lparser.c lstate.c lstring.c ltable.c ltm.c lundump.c lvm.c lzio.c lauxlib.c lbaselib.c lbitlib.c lcorolib.c ldblib.c liolib.c lmathlib.c loslib.c lstrlib.c ltablib.c loadlib.c linit.c)
set_target_properties(lualib PROPERTIES OUTPUT_NAME "lua")