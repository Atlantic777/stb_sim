#include <stdio.h>
#include "input.h"
#include "input/keyboard.h"

int main()
{
  input_t kb;
  kb_init(&kb);

  char ev;
  kb.read(&ev);

  printf("d: %hhd x: %hhx c: %c\n", ev, ev, ev);

  kb_deinit();
  return 0;
}
