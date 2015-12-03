#ifndef MODEL_H
#define MODEL_H

typedef struct model {
	int (*init)();
	int (*deinit)();

	int (*scan)();

	int (*get_current_ch)();
	int (*get_ch_list)();
  int (*get_volume)();

	int (*ch_switch)(int ch);
	int (*ch_up)();
	int (*ch_down)();

	int (*vol_up)();
	int (*vol_down)();
} model_t;

#endif
