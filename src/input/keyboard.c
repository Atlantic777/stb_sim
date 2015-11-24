#include "input/keyboard.h"
#include <stdio.h>
#include <termios.h>

static int initialized = 0;

int getch(void)
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

int kb_get_event(char *ev)
{
  if(0 != initialized)
  {
    *ev = getch();
  }
  else
  {
    puts("input not opened");
  }

  return 0;
}

int kb_init()
{
  if(0 == initialized)
  {
    initialized = 1;
    puts("open");
  }
  else
  {
    puts("already opened");
  }

  return 0;
}

int kb_deinit()
{
  if(0 != initialized)
  {
    puts("closed");
    initialized = 0;;
  }
  else
  {
    puts("no op");
  }
}
