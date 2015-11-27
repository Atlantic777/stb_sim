#include "input/keyboard.h"
#include "controller.h"
#include "view/dfb_view.h"
#include <stdio.h>

input_t       keyboard;
view_t        dfb;
controller_t  ctrl;

int main()
{
  kb_init(&keyboard);
  dfb_view_init(&dfb);

  ctrl_init(&ctrl);
  ctrl_set_view(&dfb);

  kb_set_callback(ctrl.handler);

  dfb.start();

  ctrl.start();
  keyboard.start();
}
