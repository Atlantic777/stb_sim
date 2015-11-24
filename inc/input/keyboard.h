#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "input.h"

int kb_init(input_t*);
int kb_deinit();

int kb_get_event(char *ev);
int kb_set_callback(void(*start)(char));

#endif
