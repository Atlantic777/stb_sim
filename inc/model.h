#ifndef MODEL_H
#define MODEL_H

#include "model/service_list.h"

typedef struct model {
	int (*init)(char*);
	int (*deinit)();

	int (*scan)();

	service_item_t* (*get_current_ch)();
	int (*get_ch_list)();
  int (*get_volume)();

	int (*ch_switch)(int ch);
	int (*ch_up)();
	int (*ch_down)();

	int (*vol_up)();
	int (*vol_down)();
	int (*vol_mute)();
} model_t;

#endif
