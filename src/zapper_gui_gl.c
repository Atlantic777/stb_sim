#include "zapper_gui_gl.h"
#include "GL/glut.h"
#include <string.h>
#include <pthread.h>

zapper_gui_t Zapper_gui_gl = {
  .init = zapper_gui_gl_init,
  .deinit = zapper_gui_gl_deinit,
  .draw_box = draw_box,
  .clear_box = clear_box,
  .initialized = 0,
};

#define NAME "something"
char **argv_;
int argc_;

int state_draw;

pthread_t gl_thread_id;

void *gl_thread(void *args)
{
  glutInit(&argc_, argv_);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(640, 480);
  glutCreateWindow(argv_[0]);
  glutIdleFunc(draw);
  glClearColor(0, 0, 0, 0);
  glViewport(0, 0, 300, 300);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glScalef(1, -1, 1);
  gluOrtho2D(0, 300, 0, 300);
  glMatrixMode(GL_MODELVIEW);
  glutMainLoop();
}

void draw()
{
  glClear(GL_COLOR_BUFFER_BIT);

  if(state_draw == 1)
  {
    glColor3f(0xFF, 0xFF, 0xFF);
    glBegin(GL_QUADS);
      glVertex2f(0, 0);
      glVertex2f(0, 100);
      glVertex2f(100, 100);
      glVertex2f(100, 0);
    glEnd();
  }
  glutSwapBuffers();
}

void draw_box()
{
  state_draw = 1;
}

void clear_box()
{
  state_draw = 0;
}

void zapper_gui_gl_init()
{
  if(Zapper_gui_gl.initialized == 0)
  {
    Zapper_gui_gl.initialized = 1;

    argc_ = 1;
    argv_ = malloc(sizeof(char*)*argc_);
    argv_[0] = malloc(strlen(NAME));
    strcpy(argv_[0], NAME);

    pthread_create(&gl_thread_id, NULL, gl_thread, NULL);
  }
}

void zapper_gui_gl_deinit()
{
  if(Zapper_gui_gl.initialized == 1)
  {
    Zapper_gui_gl.initialized = 0;
  }
}
