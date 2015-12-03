#include "model/service_list.h"
#include <stdlib.h>

int pat2services(pat_table_t *pat, service_table_t *srv)
{
	puts("pat2services");
	srv->cnt = 0;
	srv->items =  malloc(sizeof(service_item_t)*pat->programs_cnt);

	program_desc_t *current = pat->programs;
	int i;
	for(i = 0; i < pat->programs_cnt; i++)
	{
		printf("in the loop\n");
		printf("program_number_hdr: %d\n", current->program_number);
		if(current->program_number != 0u && current->program_number < 1024u)
		{
			srv->items[srv->cnt].position = i;
			srv->items[srv->cnt].program_number = current->program_number;
			srv->items[srv->cnt].pmt_pid = current->program_pid;

			printf("prog_number: %d - pmt pid: %d\n", 
				srv->items[srv->cnt].program_number, srv->items[srv->cnt].pmt_pid);

			srv->cnt++;
		}

		current = current->next;
	}

	return 0;
}
