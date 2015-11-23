#include "zapper_fsm.h"

State state;
char input_len = 0;
char input_buff[4];
int stopped = 1;
int current_chan = 1;
info_callback_t info_callback = NULL;

int isNum(char ev)
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

void store_num(char ev)
{
  input_buff[input_len] = ev;
  input_len++;
  input_buff[input_len] = '\0';

}

void schedule_update()
{
  state = state_update;
  zapp_fsm_key_press(SIG_KEY_TIMEOUT);
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

void zapp_fsm_key_press(char ev)
{
  state(ev);
}

int zapp_fsm_get_current_ch()
{
  return current_chan;
}

int zapp_fsm_stopped()
{
  return stopped;
}
