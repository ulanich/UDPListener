#pragma once
#include "stdint.h"

#define SYNCHRO_MARKER_SIZE (2)
#define HEADER_SIZE			(6)
#define CRC_SIZE			(2)
#define IDENTIFIER_SIZE		(2)
#define OBTS_SIZE			(4)
#define	TEXT_MESSAGE_SIZE	(4)
enum
{
	TM_OBTS = 0x0123,
	TM_TEXT = 0x0456,
	TM_SYNCHRO = 0x3e9c,
};

//#define UNCORRECTED_VER
#ifdef UNCORRECTED_VER
typedef struct
{
	uint8_t year : 6;
	uint8_t month : 4;
	uint8_t day : 5;
	uint8_t hour : 5;
	uint8_t minute : 6;
	uint8_t seconds : 6;
} tsOBTS;
#else
typedef struct
{
	uint8_t year : 6;
	uint8_t month : 4;
	uint8_t day : 5;
	uint8_t seconds : 6;
	uint8_t minute : 6;
	uint8_t hour : 5;
} tsOBTS;
#endif

typedef struct
{
	float arg;
	float sinArg;
	double doubleArg;
} tsTextMessage;

typedef struct
{
	uint16_t appProcessInd : 11;
	uint16_t secHdr : 1;
	uint16_t packetType : 1;
	uint16_t packetVersion : 3;
	uint16_t packetCounter : 14;
	uint16_t seqFlags : 2;
	uint16_t packetDataLen : 16;
}tsHeader;

typedef struct
{
	uint16_t synchro;
	uint16_t header[3];
	uint8_t data[78];
	uint16_t crc;
} tsPacket;