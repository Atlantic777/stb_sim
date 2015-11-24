#include <stdio.h>
#include "input/keyboard.h"

int main()
{
  kb_init();

  char ev;
  kb_get_event(&ev);

  printf("d: %hhd x: %hhx c: %c\n", ev, ev, ev);

  kb_deinit();
  return 0;
}
