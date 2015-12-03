#ifndef PARSER_UTILS_H
#define PARSER_UTILS_H

#include <stdint.h>

uint16_t get_short(uint8_t *buff, uint32_t offset)
{
	uint16_t up, down;
	uint16_t ret;


	up	= buff[offset] << 8;
	down	= buff[offset+1];

	ret = up + down;

	return ret;
}

#endif
