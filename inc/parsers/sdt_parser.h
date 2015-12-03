#ifndef SDT_PARSER_H
#define SDT_PARSER_H

#include <stdint.h>

typedef struct sdt_table sdt_table_t;
typedef struct service_loop_item service_loop_item_t;
typedef struct service_desc service_desc_t;

struct sdt_table {
	uint8_t table_id;
	uint16_t section_length;
	uint16_t transport_stream_id;
	uint8_t section_number;
	uint8_t last_section_number;
	uint16_t original_network_id;
	uint8_t reserved_future_use;

	uint8_t services_cnt;
	service_loop_item_t *services;
};

struct service_loop_item {
	uint16_t service_id;
	uint8_t reserved_future_use;
	uint8_t EIT_schedule_flag;
	uint8_t present_following_flag;
	uint8_t running_status;
	uint8_t free_CA_mode;

	uint16_t descriptors_loop_length;
	uint8_t descriptors_cnt;
	service_desc_t *descriptors;

	service_loop_item_t *next;
};

struct service_desc {
	uint8_t decsriptor_tag;
	uint8_t descriptor_length;
	uint8_t service_type;

	uint8_t service_provider_name_length;
	char *provider_name;

	uint8_t service_name_length;
	char *service_name;
};

int sdt_parse(sdt_table_t *sdt, uint8_t *buff);

#endif
