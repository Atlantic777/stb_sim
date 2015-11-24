#include "zapper_fsm.h"

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
