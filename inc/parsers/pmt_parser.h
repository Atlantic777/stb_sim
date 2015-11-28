#ifndef PMT_PARSER_H
#define PMT_PARSER_H

#define PMT_HEADER_LEN 12

#include <stdint.h>

typedef struct pmt_table pmt_table_t;
typedef struct stream stream_t;

struct pmt_table {
	uint8_t		table_id;
	uint16_t	section_length;
	uint16_t	program_number;
	uint16_t	version_number;
	uint8_t		section_number;
	uint8_t		last_section_number;
	uint16_t	pcr_pid;
	uint16_t	program_info_length;
	uint16_t	streams_cnt;
	stream_t	*streams;
	uint32_t	crc;
};

struct stream {
	uint8_t		stream_type;
	uint16_t	elementary_pid;
	uint16_t	es_info_length;
	stream_t	*next;
};

int pmt_parse(pmt_table_t *pmt, uint8_t *buff);
int pmt_print(pmt_table_t *pmt);
int pmt_get_audio_pid(pmt_table_t *pmt);
int pmt_get_video_pid(pmt_table_t *pmt);

#endif
