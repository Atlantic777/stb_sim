#include "parsers/init_config_parser.h"
#include <stdio.h>
#include <string.h>

static const char DELIM[] = "\n";

#define jump() c = strtok(NULL, DELIM)

int8_t parse_init_config(char *s_config, init_config_t *out)
{
	char *c = strtok(s_config, DELIM);
	sscanf(c, "[dvb-t]\n");

	jump();
	sscanf(c, "frequency=%d\n", &(out->frequency));

	jump();
	sscanf(c, "bandwidth=%d\n", &(out->bandwidth));

	char module[32];
	jump();
	sscanf(c, "module=%s\n", module);

	if(strstr(module, module_1))
	{
		out->module = INIT_DVB_T;
	}
	else
	{
		out->module = INIT_DVB_T2;
	}




	jump();
	sscanf(c, "[initial_channel]");

	jump();
	sscanf(c, "aPID=%d\n", &(out->aPID));

	jump();
	sscanf(c, "vPID=%d\n", &(out->vPID));

	char aType[32], vType[32];
	jump();
	sscanf(c, "aType=%s\n", aType);

	if(strstr(aType, audio_1))
	{
		out->aType = INIT_AC3;
	}
	else
	{
		out->aType = INIT_MPEG;
	}

	jump();
	sscanf(c, "vType=%s\n", vType);

	if(strstr(vType, video_1))
	{
		out->vType = INIT_MPEG2;
	}	
	else
	{
		out->vType = INIT_MPEG4;
	}

	return 0;
}
