#include "parsers/pmt_parser.h"
#include <stdio.h>
#include <stdlib.h>
/* #include <tdp_api.h> */

#define AUDIO_TYPE 3
#define VIDEO_TYPE 4

static uint16_t get_short(uint8_t *buff, uint32_t offset)
{
	uint16_t up, down;
	uint16_t ret;


	up	= buff[offset] << 8;
	down	= buff[offset+1];

	ret = up + down;

	return ret;
}

int pmt_parse(pmt_table_t *pmt, uint8_t *buff)
{
	uint16_t parsed = 0;
	uint16_t streams_offset;

	pmt->table_id = buff[0];
	pmt->section_length = get_short(buff, 1);
	pmt->program_number = get_short(buff, 3);
	pmt->version_number = get_short(buff, 5);
	pmt->program_info_length = get_short(buff, 11);

	// parse descriptors
	parsed = 0;
	while(parsed < pmt->program_info_length)
	{
		parsed++;
		puts("here");
	}

	// parse streams
	streams_offset =
		PMT_HEADER_LEN + pmt->program_info_length + 1;

	parsed = streams_offset;
	pmt->streams = malloc(sizeof(stream_t));
	stream_t *current= pmt->streams;
	current->next = NULL;
	pmt->streams_cnt = 0;

	while(parsed < pmt->section_length)
	{
		current->stream_type = buff[parsed];

		current->elementary_pid =
			get_short(buff, parsed+1);

		current->es_info_length =
			get_short(buff, parsed+3);

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

	int i;
	for(i = 0; i < pmt->streams_cnt; i++)
	{
		if(current->stream_type == AUDIO_TYPE)
		{
			return current->elementary_pid;
		}
		else
		{
			current = current->next;
		}
	}
}

int pmt_get_video_pid(pmt_table_t *pmt)
{
	stream_t *current = pmt->streams;

	int i;
	for(i = 0; i < pmt->streams_cnt; i++)
	{
		if(current->stream_type == VIDEO_TYPE)
		{
			return current->elementary_pid;
		}
		else
		{
			current = current->next;
		}
	}
}
