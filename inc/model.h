#ifndef MODEL_H
#define MODEL_H

int stb_init();
int stb_deinit();

int stb_scan();

int stb_get_current_ch();
int stb_get_ch_list();

int stb_ch_switch(int ch);
int stb_ch_up();
int stb_ch_down();

#endif
