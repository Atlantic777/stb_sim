#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "input.h"
#include <stdint.h>

int kb_init(input_t*);
int kb_deinit();

int kb_get_event(uint32_t *ev);
int kb_set_callback(void(*start)(uint32_t));

#endif
