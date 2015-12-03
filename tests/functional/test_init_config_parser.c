#include "parsers/init_config_parser.h"
#include <stdio.h>

int main()
{
	char config[] = "[dvb-t]\n"
		"frequency=470\n"
		"bandwidth=8\n"
		"module=DVB-T\n"
		
		"[initial_channel]\n"
		"aPID=101\n"
		"vPID=102\n"
		"aType=ac3\n"
		"vType=mpeg2\n";

	init_config_t parser_config;
	
	parse_init_config(config, &parser_config);

	return 0;
}
