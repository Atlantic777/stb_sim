#include "input/remote.h"
#include "controller.h"
#include "view/dfb_view.h"
#include "model.h"
#include <stdio.h>

input_t       remote;
view_t        dfb;
model_t	    	model;
controller_t  ctrl;

int main(int argc, char **argv)
{
	if(argc == 1)
	{
		puts("Don't have file config");
		return -1;
	}

  rc_init(&remote);
  stb_model_init(&model);

  dfb_view_init(&dfb);

  ctrl_init(&ctrl);
  ctrl_set_view(&dfb);
  ctrl_set_model(&model);

  rc_set_callback(ctrl.handler);

  dfb.start();
  ctrl.start(argv[1]);

  remote.start();
	model.deinit();
	puts("finish");
}
