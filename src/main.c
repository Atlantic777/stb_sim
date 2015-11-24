#include <stdio.h>
#include "keyboard.h"
#include "zapper_fsm.h"
#include "zapper_gui_gl.h"

char event;

void my_info_callback(int chan)
{
  Zapper_gui_gl.draw_box();
}

void my_clear_callback()
{
  Zapper_gui_gl.clear_box();
}

int main()
{
  kb_init();
  zapp_fsm_init(my_info_callback);
  Zapper_gui_gl.init();
  Zapper_gui_gl.init();

  while(!zapp_fsm_stopped())
  {
    kb_get_event(&event);
    zapp_fsm_key_press(event);
  }

  Zapper_gui_gl.deinit();
  zapp_fsm_deinit();
  kb_deinit();

  return 0;
}
