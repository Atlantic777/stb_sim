#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "input.h"

int rc_init(input_t*);
int rc_deinit();

int rc_get_event(char *ev);
int rc_set_callback(void(*start)(char));

#endif
