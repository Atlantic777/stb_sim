#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "controller.h"

static view_t   *_view;
/* static model_t  *model; */
/* static input_t  *input; */

State _state;

static void state_scan(signal_t, char*);
static void state_idle(signal_t, char*);
static void state_num_enter(signal_t, char*);

// ############ STATES ################
static void state_scan(signal_t sig, char *args)
{
  LOG("scanning...");
  _state = state_idle;
  _state(SIG_NOOP, NULL);

  LOG("scan state finished");
}

static void state_idle(signal_t sig, char *args)
{
  if(SIG_NUM_KEY == sig)
  {
    _state = state_num_enter;
  }
  LOG("Idle state...");
}

static void state_num_enter(signal_t sig, char *args)
{
  LOG("num enter");
  if(SIG_NUM_KEY == sig)
  {
    printf("number %d\n", args[0]-48);
  }
  else if(SIG_TIMEOUT == sig)
  {
    LOG("num enter timed out!");
    _state = state_idle;
  }
}

// ########### ONE SHOT ##############
static void boot()
{
  LOG("Booting...");
}

static void parse_config()
{
  LOG("Parse config...");
}

static void switch_default()
{
  LOG("Switching to default A/V streams...");
}

// ####### PUBLIC ##############
static void start()
{
  LOG("Hello from ctrl start");

  boot();
  parse_config();
  switch_default();

  _state(SIG_NOOP, NULL);
}

static void stop()
{
}

static int isNum(char c)
{
  if(c >= '0' && c <= '9')
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

static signal_t decode_signal(char ev)
{
  if(isNum(ev))
  {
    LOG("SIG_NUM_KEY");
    return SIG_NUM_KEY;
  }
  else if('q' == ev)
  {
    LOG("SIG_KILL");
    return SIG_KILL;
  }
}

static void input_handler(char ev)
{
  signal_t sig = decode_signal(ev);

  if(SIG_KILL == sig)
  {
    exit(0);
  }

  _state(sig, &ev);
}

int ctrl_init(controller_t *ctrl)
{
  ctrl->start     = start;
  ctrl->stop      = stop;
  ctrl->handler   = input_handler;

  _state          = state_scan;

  return 0;
}

int ctrl_deinit()
{
  return 1;
}

int ctrl_set_view(view_t *view)
{
  _view = view;
  return 0;
}
