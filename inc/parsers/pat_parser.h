/**
 * @file pat_parser.h
 * @author Nikola Hardi
 * @date November 2015
 * @brief Program Allocation Table - structure and parser
 */
#ifndef PAT_PARSER_H
#define PAT_PARSER_H

#include <stdint.h>

typedef struct program_desc program_desc_t;
typedef struct pat_table pat_table_t;

/**
 * @brief The program allocation table
 */
struct pat_table {
	uint8_t		table_id;
	uint8_t		section_syntax_indicator;
	uint16_t	section_length;
	uint16_t	transport_stream_id;
	uint8_t		version;
	uint8_t		current_next_indicator;
	uint8_t		section_number;
	uint8_t		last_section_number;
	uint8_t		programs_cnt;
	program_desc_t	*programs;
	uint32_t	crc;
};

/**
 * @brief A program entry in the PAT.
 */
struct program_desc {
	uint16_t	program_number;
	uint16_t	program_pid;
	program_desc_t  *next;
};

int pat_parse(pat_table_t *pat, uint8_t *buff);
void pat_print(pat_table_t *pat);
int pat_get_entry(int ch, pat_table_t *pat, program_desc_t **desc);

#endif
