#include <stdio.h>
#include <termios.h>
#include <pthread.h>
#include "input/remote.h"
#include <linux/input.h>
#include <fcntl.h>

static void (*_handler)(char);
static int initialized = 0;

static void rc_start();

pthread_t rc_thread_id;

static const char* dev = "/dev/input/event0";
static int input_file_desc;

static int getch(void)
{
	/*
  int ch;
  struct termios oldt;
  struct termios newt;
  tcgetattr(0, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(0, TCSANOW, &newt);
  ch = getchar();
  tcsetattr(0, TCSANOW, &oldt);
	*/

	struct input_event ev;
	read(input_file_desc, &ev, sizeof(struct input_event));

	if(ev.type == 1 && ev.value == 0)
	{
		printf("RemoteControl: %c\n", ev.code);
		return ev.code;
	}
	else
	{
		return 255;
	}
}

int rc_get_event(char *ev)
{
  if(0 != initialized)
  {
    *ev = getch();
		puts("ffs");
  }
  else
  {
    /* puts("input not opened"); */
  }

  return 0;
}

int rc_init(input_t *input)
{
  if(0 == initialized)
  {
    initialized = 1;
    input->start = rc_start;
    input->read = rc_get_event;

		input_file_desc = open(dev, O_RDWR);
    /* puts("open"); */
  }
  else
  {
    /* puts("already opened"); */
  }

  return 0;
}

int rc_deinit(input_t *input)
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

int rc_set_callback(void(*handler)(char))
{
  _handler = handler;
  return 0;
}

static void *rc_loop(void *args)
{
  char ev;

  while(1) {
    rc_get_event(&ev);

		if(ev != 255)
		{
			_handler(ev);
		}
  }
}

static void rc_start()
{
  pthread_create(&rc_thread_id, NULL, rc_loop, NULL);
  pthread_join(rc_thread_id, NULL);
}
