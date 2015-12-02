#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

typedef struct input {
  void (*start)();
  int  (*read)(uint32_t*);
} input_t;

#endif
