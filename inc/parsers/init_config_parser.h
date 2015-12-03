#ifndef INIT_CONFIG_PARSER_H
#define INIT_CONFIG_PARSER_H

#include <stdint.h>

#define INIT_DVB_T  0
#define INIT_DVB_T2 1
#define INIT_AC3		2
#define INIT_MPEG		3
#define INIT_MPEG2  4
#define INIT_MPEG4  5

#define module_1	"DVB-T"
#define module_2	"DVB-T2"

#define audio_1		"ac3"
#define audio_2		"mpeg"

#define video_1		"mpeg2"
#define video_2		"mpeg4"

typedef struct init_config init_config_t;

struct init_config {
	uint16_t	frequency;
	uint8_t 	bandwidth;
	uint8_t		module;

	uint16_t	aPID;
	uint16_t	vPID;

	uint8_t		aType;
	uint8_t		vType;
};

int8_t parse_init_config(char *c, init_config_t *out);

#endif
