#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <GL/glut.h>
#include <X11/Xlib.h>
#include "view/gl_view.h"
#include "afb/opengl.h"

static afb_t afb;
pthread_t th_opengl_id;

static int show_num_input = 0;
static int show_info_bar = 0;

pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER;

static void gl_show_num_input(char *buf)
{
  pthread_mutex_lock(&mx);
  show_num_input = 1;
  pthread_mutex_unlock(&mx);
}

static void gl_hide_num_input()
{
  pthread_mutex_lock(&mx);
  show_num_input = 0;
  pthread_mutex_unlock(&mx);
}

static void gl_show_info_bar(int c)
{
  pthread_mutex_lock(&mx);
  show_info_bar = 1;
  pthread_mutex_unlock(&mx);
}

static void gl_hide_info_bar()
{
  pthread_mutex_lock(&mx);
  show_info_bar = 0;
  pthread_mutex_unlock(&mx);
}

static void render_num_input()
{
  pthread_mutex_lock(&mx);
  if(show_num_input)
  {
    glBegin(GL_QUADS);
      glVertex2d(560, 40);
      glVertex2d(620, 40);
      glVertex2d(620, 20);
      glVertex2d(560, 20);
    glEnd();
  }
  pthread_mutex_unlock(&mx);
}

static void render_info_bar()
{
  if(show_info_bar)
  {
    glBegin(GL_QUADS);
      glVertex2d(20, 460);
      glVertex2d(620, 460);
      glVertex2d(620, 420);
      glVertex2d(20, 420);
    glEnd();
  }
}

static void gl_view_render()
{
  glClear(GL_COLOR_BUFFER_BIT);

  render_num_input();
  render_info_bar();

  glutSwapBuffers();
}

static void gl_view_resize(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glScalef(1, -1, 1);
  gluOrtho2D(0, w, 0, h);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

static void gl_view_log(char *message)
{
}

static void *glut_loop(void *args)
{
  /* afb_gl_init(&afb); */
  /* afb.init(); */
  XInitThreads();
  char *argv[1] = {(char*)"nothing"};
  int c = 1;
  glutInit(&c, argv);

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glClearColor(0.0, 1.0, 1.0, 1.0);

  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("something");
  glutIdleFunc(gl_view_render);
  glutReshapeFunc(gl_view_resize);

  glutMainLoop();
}

static void gl_view_start()
{
  pthread_create(&th_opengl_id, NULL, glut_loop, NULL);
  pthread_detach(th_opengl_id);
}

int gl_view_init(view_t *view)
{
  view->log = gl_view_log;
  view->start = gl_view_start;

  view->show_num_input = gl_show_num_input;
  view->hide_num_input = gl_hide_num_input;

  view->show_info_bar = gl_show_info_bar;
  view->hide_info_bar = gl_hide_info_bar;

  return 0;
}

int gl_view_deinit()
{
  return 0;
}
