#include "afb/opengl.h"
#include "GL/glut.h"

static void opengl_init()
{
  /* char *argv[1] = {(char*)"nothing"}; */
  /* int c = 1; */
  /* glutInit(&c, argv); */
  /*  */
  /* glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); */
  /* glClearColor(0.0, 0.0, 1.0, 1.0); */
}


int afb_gl_init(afb_t *afb)
{
  afb->init = opengl_init;
  // TODO: double buffering support
  /* TODO: afb->clear = opengl_clear; */
  /* afb->write_text = opengl_write_text; */
  /* afb->set_color = opengl_set_color; */
  /* afb->draw_rect = opengl_draw_rect; */
  return 0;
}

int afb_gl_deinit()
{
  return 0;
}
