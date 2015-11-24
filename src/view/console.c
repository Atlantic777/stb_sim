#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include "view/console.h"

static pthread_t console_id;

static char nums[3];
static int  nums_len;
static int  chan;

static int s_input    = 0;
static int s_info_bar = 0;
static int s_volume   = 0;

static void console_log(char *message)
{
  printf("LOG: %s\n", message);
}

static void render_num_input()
{
  if(s_input)
  {
    int c;
    for(c = nums_len; c < 3; c++)
    {
      nums[c] = '-';
    }

    printf("Current input: %c %c %c\n", nums[0], nums[1], nums[2]);
  }
  else
  {
    puts("");
  }
}

static void render_info_bar()
{
  if(s_info_bar)
  {
    printf("Current chan is: %d\n", chan);
  }
  else
  {
    puts("");
  }
}

static void render_volume_info()
{
}

static void *console_render_loop(void *args)
{
  while(1)
  {
    system("clear");

    render_num_input();
    render_info_bar();
    /* render_volume_info(); */

    puts("here");

    sleep(1);
  }
}

static void console_start()
{
  pthread_create(&console_id, NULL, console_render_loop, NULL);
  pthread_detach(console_id);
}

static void show_num_input(char *c)
{
  strncpy(nums, c, 3);
  nums_len = strlen(nums);
  s_input = 1;
}

static void hide_num_input()
{
  s_input = 0;
}

static void show_info_bar(int ch)
{
  chan = ch;
  s_info_bar = 1;
}

static void hide_info_bar()
{
  s_info_bar = 0;
}

int console_init(view_t *view)
{
  view->log = console_log;
  view->start = console_start;

  view->show_num_input = show_num_input;
  view->hide_num_input = hide_num_input;

  view->show_info_bar = show_info_bar;
  view->hide_info_bar = hide_info_bar;
  return 0;
}

int console_deinit()
{
  return 0;
}
