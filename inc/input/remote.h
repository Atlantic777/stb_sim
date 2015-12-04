/**
 * @file remote.h
 * @author Nikola Hardi
 * @date November 2015
 * @brief Remote control (implements input interface)
 */
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "input.h"
#include <stdint.h>

int rc_init(input_t*);
int rc_deinit();

int rc_get_event(uint32_t *ev);
int rc_set_callback(void(*start)(uint32_t));

#endif
