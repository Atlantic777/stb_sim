#include "zapper_fsm.h"
#include <stdio.h>
#include <pthread.h>
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

char input_buff[4];
char input_len = 0;

static pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;

void *schedule_update(void *args);

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
  gettimeofday(&last_key_press, NULL);

  future.tv_sec = last_key_press.tv_sec + WAIT_INTERVAL;
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

void *schedule_update(void *args)
{
  puts("starting a timer");
  pthread_mutex_lock(&mx);

  struct timeval now;
  struct timespec future;
  gettimeofday(&now, NULL);
  future.tv_sec = now.tv_sec + 3;
  future.tv_nsec = now.tv_usec * 1000;
  int ret;

  while(ret != ETIMEDOUT)
  {
    ret = pthread_cond_timedwait(&cond, &mx, &future);
  }

  if(ret == ETIMEDOUT && should_update())
  {
    state = STATE_UPDATE;
    zapp_fsm_key_press(-1);
  }
  else
  {
    puts("killing timer");
  }

  pthread_mutex_unlock(&mx);
}

void zapp_fsm_key_press(char ev)
{
  if(STATE_IDLE == state)
  {
    if(isNum(ev))
    {
      state = STATE_NUM_ENTER;
      store_num(ev);

      pthread_t notifier_handle;
      pthread_create(&notifier_handle, NULL, schedule_update, NULL);
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
    process_buffer();
    info_callback(current_chan);
    state = STATE_IDLE;

    /* pthread_mutex_lock(&mx); */
    /* pthread_cond_signal(&cond); */
    /* pthread_mutex_unlock(&mx); */
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
