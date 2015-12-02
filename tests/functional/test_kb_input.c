#include <stdio.h>
#include "input.h"
#include "input/keyboard.h"
#include <stdint.h>

int main()
{
  input_t kb;
  kb_init(&kb);

  uint32_t ev;
  kb.read(&ev);

  printf("d: %hhd x: %hhx c: %c\n", ev, ev, ev);

  kb_deinit();
  return 0;
}
