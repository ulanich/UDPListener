#pragma once
#ifndef CRC_H_
#define CRC_H_

#include <stddef.h>
#include <stdint.h>

uint16_t crc_calc(const uint8_t* data, size_t len);

static inline uint16_t crc_init(void)
{
	return 0;
}

static inline uint16_t crc_next(uint16_t crc, uint8_t data)
{
	uint16_t eor;
	unsigned int i = 8;

	crc ^= (uint16_t)data << 8;
	do {
		/* This might produce branchless code */
		eor = crc & 0x8000 ? 0x1021 : 0;
		crc <<= 1;
		crc ^= eor;
	} while (--i);

	return crc;
}

static inline uint16_t crc_final(uint16_t crc)
{
	return crc;
}
#endif
