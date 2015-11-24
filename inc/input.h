#ifndef INPUT_H
#define INPUT_H

typedef struct input {
  void (*start)();
  int  (*read)(char*);
} input_t;

#endif
