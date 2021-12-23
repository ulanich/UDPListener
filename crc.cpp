#include "crc.h"
#include <iostream>

uint16_t crc_calc(const uint8_t *data, size_t len)
{
	uint16_t crc = crc_init();

	if (len) do {
		crc = crc_next(crc, *data++);
	} while (--len);

	return crc_final(crc);
}

