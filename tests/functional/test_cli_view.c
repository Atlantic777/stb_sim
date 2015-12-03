#include "input/keyboard.h"
#include "controller.h"
#include "view/console.h"
#include <stdio.h>

input_t       keyboard;
view_t        console;
controller_t  ctrl;

int main()
{
  kb_init(&keyboard);
  console_init(&console);

  ctrl_init(&ctrl);
  ctrl_set_view(&console);

  kb_set_callback(ctrl.handler);

  console.start();

  ctrl.start(NULL);
  keyboard.start();
}
