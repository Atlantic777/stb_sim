#include <stdio.h>
#include <termios.h>
#include <pthread.h>
#include "input/keyboard.h"
#include "input/keyboard_codes.h"
#include "controller/norm_codes.h"

static void (*_handler)(uint32_t);
static int initialized = 0;

static void kb_start();

pthread_t kb_thread_id;

static int isNum(uint32_t ev)
{
  if(ev >= '0' && ev <= '9')
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

static char normalize_code(uint32_t code)
{
  if(KB_KEY_EXIT == code)
  {
    return N_KEY_EXIT;
  }
  else if(isNum(code))
  {
    return code-48;
  }
}

static uint32_t getch(void)
{
  int ch;
  struct termios oldt;
  struct termios newt;
  tcgetattr(0, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(0, TCSANOW, &newt);
  ch = getchar();
  tcsetattr(0, TCSANOW, &oldt);
  return ch;
}

int kb_get_event(uint32_t *ev)
{
  if(0 != initialized)
  {
    *ev = getch();
  }
  else
  {
    /* puts("input not opened"); */
  }

  return 0;
}

int kb_init(input_t *input)
{
  if(0 == initialized)
  {
    initialized = 1;
    input->start = kb_start;
    input->read = kb_get_event;
    /* puts("open"); */
  }
  else
  {
    /* puts("already opened"); */
  }

  return 0;
}

int kb_deinit(input_t *input)
{
  if(0 != initialized)
  {
    /* puts("closed"); */
    initialized = 0;;
  }
  else
  {
    /* puts("no op"); */
  }

  return 0;
}

int kb_set_callback(void(*handler)(uint32_t))
{
  _handler = handler;
  return 0;
}

static void *kb_loop(void *args)
{
  uint32_t ev;

  while(1) {
    kb_get_event(&ev);
    _handler(normalize_code(ev));
  }
}

static void kb_start()
{
  pthread_create(&kb_thread_id, NULL, kb_loop, NULL);
  pthread_join(kb_thread_id, NULL);
}
