#include "fix16.h"
#include <stdbool.h>
#ifndef FIXMATH_NO_CTYPE
#include <memory.h>
#include <stdio.h>

#else
static inline int isdigit(int c)
{
    return c >= '0' && c <= '9';
}

static inline int isspace(int c)
{
    return c == ' ' || c == '\r' || c == '\n' || c == '\t' || c == '\v' || c == '\f';
}
#endif

static const uint32_t scales[8] = {
    /* 5 decimals is enough for full fix16_t precision */
    1, 10, 100, 1000, 10000, 100000, 100000, 100000
};

static char *itoa_loop(char *buf, uint32_t scale, uint32_t value, bool skip)
{
    while (scale)
    {
        unsigned digit = (value / scale);
    
        if (!skip || digit || scale == 1)
        {
            skip = false;
            *buf = '0' + digit;
            buf++;
            value %= scale;
        }
        
        scale /= 10;
    }
    return buf;
}

size_t fix16_to_str(fix16_t value, char *buf) {
	size_t start = (size_t) buf;

	uint32_t uvalue = (value >= 0) ? value : -value;
    if (value < 0)
        *buf++ = '-';

    /* Separate the integer and decimal parts of the value */
    unsigned intpart = uvalue >> 16;
    uint32_t fracpart = uvalue & 0xFFFF;
    uint32_t scale = scales[4];
    fracpart = fix16_mul(fracpart, scale);
    
    if (fracpart >= scale)
    {
        /* Handle carry from decimal part */
        intpart++;
        fracpart -= scale;    
    }

		while (scale > 1 && fracpart % 10 == 0) {
			fracpart /= 10;
			scale /= 10;
		}
    
    /* Format integer part */
    buf = itoa_loop(buf, 10000, intpart, true);

    /* Format decimal part (if any) */
    if (scale != 1)
    {
        *buf++ = '.';
        buf = itoa_loop(buf, scale / 10, fracpart, false);
    }

  *buf = '\0';
	return (size_t) buf - start;
}

static inline int read_bit(int var, int pos) {
	return (var & (1 << pos)) != 0;
}

extern size_t fix16_to_strx(fix16_t value, char *buf) {
	size_t start = (size_t) buf;

	for (int i = 0; i < 8; i++) {
		int bitIndex = (7 - i) * 4;
		int v = ((read_bit(value, bitIndex + 3) << 3) + (read_bit(value, bitIndex + 2) << 2) + (read_bit(value, bitIndex + 1) << 1) + read_bit(value, bitIndex));

		*buf++ = (v > 9 ? ('a' + (v - 10)) : ('0' + v));

		if (i == 3) {
			*buf++ = '.';
		}
	}

	*buf = '\0';
	return (size_t) buf - start;
}