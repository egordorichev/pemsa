#ifndef strtofix16_h
#define strtofix16_h

#include <fix16.h>
#include <stddef.h>

fix16_t strtofix16(const char *nptr, char** endptr);
size_t fix16tostr(fix16_t value, char *buf);

#endif