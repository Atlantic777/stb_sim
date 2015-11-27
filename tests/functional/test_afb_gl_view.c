#include "input/keyboard.h"
#include "controller.h"
#include "view/gl_view.h"
#include <GL/glut.h>

input_t       keyboard;
view_t        gl_view;
controller_t  ctrl;

void draw()
{
  puts("here");
  glClear(GL_COLOR_BUFFER_BIT);
}

int main(int argc, char **argv)
{
  kb_init(&keyboard);
  gl_view_init(&gl_view);

  ctrl_init(&ctrl);
  ctrl_set_view(&gl_view);

  kb_set_callback(ctrl.handler);

  gl_view.start();

  ctrl.start();
  keyboard.start();

  return 0;
}
