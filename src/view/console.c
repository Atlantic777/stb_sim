#include "view/console.h"
#include <stdio.h>

static void console_log(char *message)
{
  printf("LOG: %s\n", message);
}

int console_init(view_t *view)
{
  view->log = console_log;
  return 0;
}

int console_deinit()
{
  return 0;
}
