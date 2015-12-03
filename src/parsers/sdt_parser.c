#include "parsers/sdt_parser.h"
#include "parsers/parser_utils.h"
#include <stdlib.h>

int sdt_parse(sdt_table_t *sdt, uint8_t *buff)
{
	sdt->table_id = buff[0];
	sdt->section_length = get_short(buff, 1) & 0x0FFF;
	sdt->transport_stream_id = get_short(buff, 3);
	sdt->section_number = buff[6];
	sdt->last_section_number = buff[7];
	sdt->original_network_id = get_short(buff, 8);
	sdt->reserved_future_use = buff[10];

	// first service_id is at 11
	sdt->services_cnt = 0;
	sdt->services = malloc(sizeof(service_item_t));
	service_item_t *current_item = sdt->services;
	service_desc_t *current_desc;

	int cursor = 11;
	int descriptor_data_target;

	while(cursor*8 < sdt->section_length)
	{
		current_item->service_id = get_short(buff, cursor);
		current_item->descriptors_loop_length = get_short(buff, cursor+3) & 0x0FFF;

		current_item->descriptors_cnt = 0;
		current_item->descriptors = malloc(sizeof(service_desc_t));

		current_desc = current_item->descriptors;

		descriptor_data_target = cursor + current_item->descriptors_loop_length;

		while(cursor < descriptor_data_target)
		{
			// parse descritpro

			// move current descriptor ptr
		}

		// create new service item
		// move current service item ptr
	}
	return 0;
}
