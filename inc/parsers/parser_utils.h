/**
 * @file parser_utils.h
 * @author Nikola Hardi
 * @date November 2015
 * @brief Common usefull functions for MPEG/DVB data parsing
 */
#ifndef PARSER_UTILS_H
#define PARSER_UTILS_H

#include <stdint.h>

uint16_t get_short(uint8_t *buff, uint32_t offset);

#endif
