#include "parsers/pat_parser.h"
#include <stdio.h>
#include <stdlib.h>

static uint8_t HEADER_LEN 	= 8;
static uint8_t PROG_DESC_LEN	= 4;

static uint16_t parse_section_len(uint8_t *buff);
static uint16_t parse_stream_id(uint8_t *buff);

int pat_parse(pat_table_t *pat, uint8_t *buff)
{
	pat->table_id = buff[0];
	pat->section_length = parse_section_len(buff+1);

	uint16_t target_len;
	target_len = pat->section_length;
	uint16_t offset = HEADER_LEN;

	pat->programs_cnt = 0;
	
	pat->programs = malloc(sizeof(program_desc_t));
	program_desc_t *current = pat->programs;
	current->next = NULL;

	uint16_t prog_down, prog_up, prog;
	uint16_t pid_down, pid_up, pid;

	// TODO: too much channels (it's 8, not 9)
	while(offset < target_len)
	{
		prog_up		= buff[offset] << 8;
		prog_down	= buff[offset+1];
		prog		= prog_up + prog_down;	

		pid_up		= buff[offset+2] << 8;
		pid_down	= buff[offset+3];
		pid		= pid_up + pid_down;
		
		current->program_number = prog;
		current->program_pid = pid;
		offset += PROG_DESC_LEN;

		current->next = 
			malloc(sizeof(program_desc_t));
		current = current->next;
		current->next = NULL;

		pat->programs_cnt++;
	} 

	return 0;
}

uint16_t parse_section_len(uint8_t *buff)
{
	uint16_t up, down;
	uint16_t ret;

	up   = buff[0] & 0x0F;
	down = buff[1];

	ret = (up << 8) + down;

	return ret;
}

uint16_t parse_stream_id(uint8_t *buff)
{
	uint16_t up, down;
	uint16_t ret;

	up	= buff[0];
	down	= buff[1]; 

	ret = up + down;
	
	return ret;
}

void pat_print(pat_table_t *pat)
{
	printf("table_id\t-\t%d\n", pat->table_id);
	printf("programs_cnt\t-\t%d\n", pat->programs_cnt);

	program_desc_t *current;
	current = pat->programs;

	int i;
	for(i = 0; i < pat->programs_cnt; i++)
	{
		printf("-> program_number\t-\t%d\n", current->program_number);
		printf("-> program_pid\t-\t%d\n", current->program_pid);

		current = current->next;
	}
}

int pat_get_entry(int ch, pat_table_t *pat, program_desc_t **desc)
{
	*desc = pat->programs->next;

	int i;
	for(i = 0; i < pat->programs_cnt; i++)
	{
		*desc	= (*desc)->next;
	}

	return 0;
}
