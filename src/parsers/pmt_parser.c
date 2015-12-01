#include "parsers/pmt_parser.h"
#include <stdio.h>
#include <stdlib.h>

#define AUDIO_TYPE 3
#define VIDEO_TYPE 2

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

	puts("############# PMT parse process... ###########");

	pmt->table_id = buff[0];
	pmt->section_length = get_short(buff, 1) & 0x0FFF;
	printf("Section length: %d\n", pmt->section_length);

	pmt->program_number = get_short(buff, 3);
	printf("Program number: %d\n", pmt->program_number);

	//pmt->version_number = get_short(buff, 5) & 0x0;
	pmt->program_info_length = get_short(buff, 10) & 0x0FFF;
	printf("Program info len: %d\n", pmt->program_info_length);

	// parse descriptors
	/*
	parsed = 0;
	while(parsed < pmt->program_info_length)
	{
		parsed += 8;
		puts("here");
	}
	*/

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
		printf("parsed: %d\n", parsed);

		current->stream_type = buff[parsed];

		current->elementary_pid =
			get_short(buff, parsed+1) & 0x1FFF;

		current->es_info_length =
			get_short(buff, parsed+3) & 0x0FFF;

		printf("es_info_len %d\n", current->es_info_length);
		printf("--------------\n");
	

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
		if(current == NULL)
			break;

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
