#include "strtofix16.h"
#include <errno.h>
#include <fix16.h>
#include <stddef.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>

#define isbdigit(c) (c == '0' || c == '1')

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

fix16_t strtofix16(const char *nptr, char** endptr) {
	while (isspace(*nptr))
		nptr++;

	/* Decode the sign */
	bool negative = (*nptr == '-');
	if (*nptr == '+' || *nptr == '-')
		nptr++;

	/* Decode the number */
	fix16_t value;
	uint32_t intpart = 0;
	uint32_t fracpart = 0;
	uint32_t scale = 1;
	int count = 0;

	/* Decode hexadecimal */
	if ((nptr[0] == '0') && (( nptr[1] == 'x') || ( nptr[1] == 'X' ) ))
	{
		nptr++;
		nptr++;

		/* Decode the integer part */
		while (isxdigit(*nptr))
		{
			intpart <<= 4;
			char c = *nptr++;
			intpart += (c > '9')? (c &~ 0x20) - 'A' + 10: (c - '0');;
			count++;
		}

		value = intpart << 16;

		/* Decode the decimal part */
		if (*nptr == '.' || *nptr == ',')
		{
			nptr++;

			while (isxdigit(*nptr) && scale < 10000)
			{
				scale *= 16;
				fracpart *= 16;
				char c = *nptr++;
				fracpart += (c > '9')? (c &~ 0x20) - 'A' + 10: (c - '0');;
			}

			value += fix16_div(fracpart, scale);
		}
	} else if ((nptr[0] == '0') && (( nptr[1] == 'b') || ( nptr[1] == 'B' ) )) {
		nptr++;
		nptr++;

		/* Decode the integer part */
		while (isbdigit(*nptr)) {
			intpart <<= 1;
			char c = *nptr++;
			intpart += (c - '0');
			count++;
		}

		value = intpart << 16;

		/* Decode the decimal part */
		if (*nptr == '.' || *nptr == ',') {
			nptr++;

			while (isbdigit(*nptr) && scale < 10000) {
				scale *= 2;
				fracpart *= 2;
				char c = *nptr++;
				fracpart += (c - '0');
			}

			value += fix16_div(fracpart, scale);
		}
	} else {
		/* Decode the integer part */
		while (isdigit(*nptr)) {
			intpart *= 10;
			intpart += *nptr++ - '0';
			count++;
		}

		value = intpart << 16;

		/* Decode the decimal part */
		if (*nptr == '.' || *nptr == ',') {
			nptr++;

			while (isdigit(*nptr) && scale < 10000) {
				scale *= 10;
				fracpart *= 10;
				fracpart += *nptr++ - '0';
			}

			value += fix16_div(fracpart, scale);
		}
	}

	// Skip extra digits
	while (*nptr != '\0') {
		nptr++;
	}

	if (endptr) *endptr = (char*) nptr;
	return negative ? -value : value;
}