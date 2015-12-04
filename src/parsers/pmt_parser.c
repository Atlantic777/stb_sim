#include "parsers/pmt_parser.h"
#include "parsers/parser_utils.h"
#include <stdio.h>
#include <stdlib.h>

#define AUDIO_TYPE 3
#define VIDEO_TYPE 2

int pmt_parse(pmt_table_t *pmt, uint8_t *buff)
{
	uint16_t parsed = 0;
	uint16_t streams_offset;

	pmt->table_id = buff[0];
	pmt->section_length = get_short(buff, 1) & 0x0FFF;

	pmt->program_number = get_short(buff, 3);

	pmt->program_info_length = get_short(buff, 10) & 0x0FFF;

  // parse descriptors
  parsed = 0;

	// parse streams
	streams_offset =
		PMT_HEADER_LEN + (pmt->program_info_length);
	parsed = streams_offset;

	pmt->streams = malloc(sizeof(stream_t));
	stream_t *current= pmt->streams;
	current->next = NULL;

	pmt->streams_cnt = 0;
	while(parsed*8 < pmt->section_length)
	{
		current->stream_type = buff[parsed];

		current->elementary_pid =
			get_short(buff, parsed+1) & 0x1FFF;

		current->es_info_length =
			get_short(buff, parsed+3) & 0x0FFF;

		// jump over descriptors
		parsed += current->es_info_length + 5;

		current->next = malloc(sizeof(stream_t));
		current = current->next;
		current->next = NULL;

		pmt->streams_cnt++;
	}
}

int pmt_print(pmt_table_t *pmt)
{
	puts("printing pmt");
	printf("streams cnt\t-\t%d\n", pmt->streams_cnt);

	stream_t *current = pmt->streams;

	int i;
	for(i = 0; i < pmt->streams_cnt; i++)
	{
		printf("-> stream type\t-\t%d\n", current->stream_type);
		printf("   elem PID\t-\t%d\n", current->elementary_pid);

		current = current->next;
	}
}

int pmt_get_audio_pid(pmt_table_t *pmt)
{
	stream_t *current = pmt->streams;
	int retval = -1;

	int i;
	for(i = 0; i < pmt->streams_cnt; i++)
	{
		if(current->stream_type == AUDIO_TYPE)
		{
			retval = current->elementary_pid;
		}
		else
		{
			current = current->next;
		}
	}

	return retval;
}

int pmt_get_video_pid(pmt_table_t *pmt)
{
	stream_t *current = pmt->streams;
	int retval = -1;

	int i;
	for(i = 0; i < pmt->streams_cnt; i++)
	{
		if(current == NULL)
			break;

		if(current->stream_type == VIDEO_TYPE)
		{
			retval = current->elementary_pid;
			break;
		}
		else
		{
			current = current->next;
		}
	}

	return retval;
}
