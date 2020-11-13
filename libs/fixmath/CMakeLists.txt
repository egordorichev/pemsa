cmake_minimum_required(VERSION 2.6)
project(fixmath)
set(CMAKE_CXX_FLAGS_DEBUG "-g")

if (WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_library(fixmath STATIC fix16.c fix16_exp.c fix16_sqrt.c fix16_str.c fix16_trig.c fract32.c)