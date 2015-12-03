#include "input/keyboard.h"
#include <stdio.h>
#include "controller.h"
#include "view/console.h"

#define CHECK(F) if(F != 0) {                             \
  deinit();                                               \
  printf("ERROR! -> %s: %d\n", __FUNCTION__, __LINE__);   \
}

controller_t  ctrl;
view_t        console;
input_t       keyboard;

void init();
void deinit();

void init()
{
  CHECK( kb_init(&keyboard) );
  CHECK( ctrl_init(&ctrl) );
  CHECK( console_init(&console) );
}

void deinit()
{
  kb_deinit();
  ctrl_deinit();
  console_deinit();
}

int main()
{
  puts("Enable CTRL logging in controller.h!");
  init();

  ctrl_set_view(&console);
  kb_set_callback(ctrl.handler);

  ctrl.start(NULL);
  keyboard.start();

  deinit();
}
