#include "input/keyboard.h"
#include "controller.h"
#include "view/gl_view.h"
#include "model/fake_model.h"
#include <stdio.h>

input_t       keyboard;
view_t        view;
model_t	    	model;
controller_t  ctrl;

int main()
{
  kb_init(&keyboard);
  stb_fake_model_init(&model);

  gl_view_init(&view);

  ctrl_init(&ctrl);
  ctrl_set_view(&view);
  ctrl_set_model(&model);

  kb_set_callback(ctrl.handler);

  view.start();

  ctrl.start();
  keyboard.start();
	model.deinit();
	puts("finish");
}
