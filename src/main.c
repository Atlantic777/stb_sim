#include <stdio.h>
#include "keyboard.h"
#include "zapper_fsm.h"

char event;

void my_info_callback(int chan)
{
  printf("Current chan: %d\n", chan);
}

int main()
{
  kb_init();
  zapp_fsm_init(my_info_callback);

  while(!zapp_fsm_stopped())
  {
    kb_get_event(&event);
    zapp_fsm_key_press(event);
  }

  zapp_fsm_deinit();
  kb_deinit();

  return 0;
}
