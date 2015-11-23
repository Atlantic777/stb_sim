#include "zapper_fsm.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>

#define WAIT_INTERVAL 3

static int current_chan = 1;
typedef void(*info_callback_t)(int chan);

static info_callback_t info_callback;

static struct sigevent sev;
static timer_t timer_id;
static struct itimerspec fTime;
static struct itimerspec oTime;

static State state;

char input_buff[4];
char input_len = 0;

static int stopped;

void schedule_update();

static int isNum(char ev)
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

void process_buffer()
{
  current_chan = atoi(input_buff);
  input_len = 0;
}

int zapp_fsm_init(info_callback_t callback)
{
  sev.sigev_notify = SIGEV_THREAD;
  sev.sigev_notify_function = schedule_update;
  sev.sigev_notify_attributes = NULL;
  timer_create(CLOCK_REALTIME, &sev, &timer_id);

  state = state_idle;
  info_callback = callback;

  stopped = 0;

  return 0;
}

int  zapp_fsm_deinit()
{
  return 0;
}

void store_num(char ev)
{
  input_buff[input_len] = ev;
  input_len++;
  input_buff[input_len] = '\0';

}

void schedule_update(void *args)
{
  state = state_update;
  zapp_fsm_key_press(SIG_KEY_TIMEOUT);
}

void zapp_fsm_key_press(char ev)
{
  state(ev);
}

void state_idle(char ev)
{
  if(isNum(ev))
  {
    state = state_num_enter;
    store_num(ev);

    fTime.it_value.tv_sec = 2;
    timer_settime(&timer_id, 0, &fTime, NULL);
  }
  else if('q' == ev)
  {
    state = state_stop;
    zapp_fsm_key_press(SIG_NOOP);
  }
  else if('i' == ev)
  {
    info_callback(current_chan);
  }
}

void state_num_enter(char ev)
{
  if(isNum(ev))
  {
    store_num(ev);

    if(input_len == 3)
    {
      state = state_update;
      zapp_fsm_key_press(SIG_NOOP);
    }
  }
  else
  {
    input_len = 0;
    state = state_idle;
  }
}

void state_stop(char ev)
{
  stopped = 1;
}

void state_update(char ev)
{
  fTime.it_value.tv_sec = 0;
  timer_settime(&timer_id, 0, &fTime, NULL);
  process_buffer();
  info_callback(current_chan);
  state = state_idle;
}

int zapp_fsm_get_current_ch()
{
  return current_chan;
}

int zapp_fsm_stopped()
{
  return stopped;
}
