#include <stdio.h>
#include <termios.h>
#include <pthread.h>
#include <fcntl.h>
#include <linux/input.h>
#include "input/remote.h"
#include "input/remote_codes.h"
#include "controller/norm_codes.h"

static void (*_handler)(uint32_t);
static int initialized = 0;

static void rc_start();

pthread_t rc_thread_id;

static const char* dev = "/dev/input/event0";
static int input_file_desc;

static uint32_t getch()
{
	struct input_event ev;
	read(input_file_desc, &ev, sizeof(struct input_event));

	if(ev.type == 1 && ev.value == 0)
	{
		return ev.code;
	}
	else
	{
		return 255;
	}
}

static uint32_t to_number(uint32_t ev)
{
	if(ev >= 2 && ev <= 10)
	{
		return ev-1;
	}
	else
	{
		return 0;
	}
}

static uint32_t normalize(uint32_t ev)
{
	if(ev >= 2 && ev <= 11)
	{
		return to_number(ev);
	}
	else if(RC_KEY_EXIT == ev)
	{
		return  N_KEY_EXIT;
	}
	else if(RC_KEY_CH_UP == ev)
	{
		return N_KEY_CH_UP;
	}
	else if(RC_KEY_CH_DN == ev)
	{
		return N_KEY_CH_DN;
	}
	else if(RC_KEY_VOL_UP == ev)
	{
		return N_KEY_VOL_UP;
	}
	else if(RC_KEY_VOL_DN == ev)
	{
		return N_KEY_VOL_DN;
	}
	else if(RC_KEY_INFO == ev)
	{
		return N_KEY_INFO;
	}
}

int rc_get_event(uint32_t *ev)
{
  if(0 != initialized)
  {
    *ev = normalize(getch());
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

int rc_set_callback(void(*handler)(uint32_t))
{
  _handler = handler;
  return 0;
}

static void *rc_loop(void *args)
{
  uint32_t ev;

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
