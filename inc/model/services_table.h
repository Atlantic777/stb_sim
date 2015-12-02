#ifndef SERVICES_TABLE_H
#define SERVICES_TABLE_H

typedef struct service_item service_item_t;

struct service_item_t {
	uint8_t position;
	uint16_t program_number;
	uint16_t pmt_pid;

	uint16_t default_audio_pid;
	uint16_t default_video_pid;

	char *service_name;
	char *service_provider_name;
	uint8_t service_type;
};

#endif
