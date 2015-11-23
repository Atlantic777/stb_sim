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

static int state = STATE_IDLE;
static int current_chan = 1;
typedef void(*info_callback_t)(int chan);

static info_callback_t info_callback;
static struct timeval last_key_press;
static struct timespec future;

static struct sigevent sev;
static timer_t timer_id;
static struct itimerspec fTime;
static struct itimerspec oTime;

char input_buff[4];
char input_len = 0;

static pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;

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

int  zapp_fsm_init(info_callback_t callback)
{
  pthread_mutex_init(&mx, NULL);
  pthread_cond_init(&cond, NULL);

  sev.sigev_notify = SIGEV_THREAD;
  sev.sigev_notify_function = schedule_update;
  sev.sigev_notify_attributes = NULL;

  timer_create(CLOCK_REALTIME, &sev, &timer_id);
  puts("timer init");

  info_callback = callback;
  return 0;
}

int  zapp_fsm_deinit()
{
  pthread_cond_destroy(&cond);
  pthread_mutex_destroy(&mx);
  return 0;
}

void store_num(char ev)
{
  input_buff[input_len] = ev;
  input_len++;
  input_buff[input_len] = '\0';

}

int should_update()
{
  if(STATE_NUM_ENTER == state && input_len < 3)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

void schedule_update(void *args)
{
  if(should_update())
  {
    puts("timer fired");
    state = STATE_UPDATE;
    zapp_fsm_key_press(-1);
  }
  else
  {
    puts("misfired timer");
  }
}

void zapp_fsm_key_press(char ev)
{
  if(STATE_IDLE == state)
  {
    if(isNum(ev))
    {
      state = STATE_NUM_ENTER;
      store_num(ev);

      fTime.it_value.tv_sec = 2;
      timer_settime(&timer_id, 0, &fTime, NULL);
    }
    else if('q' == ev)
    {
      state = STATE_STOP;
    }
    else if('i' == ev)
    {
      info_callback(current_chan);
    }
  }
  else if(STATE_NUM_ENTER == state)
  {
    if(isNum(ev))
    {
      store_num(ev);

      if(input_len == 3)
      {
        state = STATE_UPDATE;
        zapp_fsm_key_press(-1);
      }
    }
    else
    {
      input_len = 0;
      state = STATE_IDLE;
    }
  }
  else if(STATE_UPDATE == state)
  {
    fTime.it_value.tv_sec = 0;
    timer_settime(&timer_id, 0, &fTime, NULL);
    process_buffer();
    info_callback(current_chan);
    state = STATE_IDLE;
  }
}

int zapp_fsm_get_current_ch()
{
  return current_chan;
}

int zapp_fsm_stopped()
{
  if(STATE_STOP == state)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
