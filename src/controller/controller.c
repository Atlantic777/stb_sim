#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "controller.h"
#include "model.h"

static view_t   *_view;
static model_t  *_model; 
State _state;

static void state_scan(signal_t, char);
static void state_idle(signal_t, char);
static void state_num_enter(signal_t, char);

static char nums[4];
static int  nums_len;
static int  current_chan = 1;

static struct itimerspec its;
static struct sigevent sev;

static timer_t t_num_input_id;
static timer_t t_info_bar_id;
static timer_t t_volume_id;

static void process_input_buffer()
{
  current_chan = atoi(nums);
  nums_len = 0;
}

static void info_bar_finish()
{
  _view->hide_info_bar();
}

static void num_finish()
{
  _view->hide_num_input();
  process_input_buffer();

  _view->show_info_bar(current_chan);
  timer_settime(t_info_bar_id, 0, &its, NULL);

  _state = state_idle;
  _state(SIG_NOOP, 0);
}

static void store_num(char n)
{
  if(nums_len == 3)
  {
    nums_len = 0;
  }

  nums[nums_len] = n;
  nums_len++;
  nums[nums_len] = 0;

  timer_settime(t_num_input_id, 0, &its, NULL);
  _view->show_num_input(nums);
	_model->ch_switch(n);
}

// ############ STATES ################
static void state_scan(signal_t sig, char args)
{
  LOG("scanning...");
  _state = state_idle;
  _state(SIG_NOOP, 0);

  LOG("scan state finished");
}

static void state_idle(signal_t sig, char args)
{
	LOG("idle state handler got key");
  if(SIG_NUM_KEY == sig)
  {
    store_num(args);
    _state = state_num_enter;
  }
  LOG("Idle state...");
}

static void state_num_enter(signal_t sig, char args)
{
  LOG("num enter");
  if(SIG_NUM_KEY == sig)
  {
    // printf("number %d\n", args[0]-48);
    store_num(args);
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
	_model->init();
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

  _state(SIG_NOOP, 0);
}

static void stop()
{
}

static int isNum(char c)
{
  if(c >= 2 && c <= 10)
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
	// TODO: create key codes table
  if(isNum(ev))
  {
    LOG("SIG_NUM_KEY");
    return SIG_NUM_KEY;
  }
  else if(116 == ev)
  {
    LOG("SIG_KILL");
    return SIG_KILL;
  }
}

static void input_handler(char ev)
{
	LOG("input handler triggered");
	puts("here");

  signal_t sig = decode_signal(ev);

	printf("key %hhd\n", ev);

  if(SIG_KILL == sig)
  {
		_model->deinit();
    exit(0);
  }

  _state(sig, ev);
}

int ctrl_init(controller_t *ctrl)
{
  ctrl->start     = start;
  ctrl->stop      = stop;
  ctrl->handler   = input_handler;

  _state          = state_scan;

  sev.sigev_notify          = SIGEV_THREAD;
  sev.sigev_notify_attributes = NULL;

  its.it_value.tv_sec = 3;
  its.it_value.tv_nsec = 0;
  its.it_interval.tv_sec = 0;
  its.it_interval.tv_nsec = 0;

  sev.sigev_notify_function = num_finish;
  timer_create(CLOCK_REALTIME, &sev, &t_num_input_id);

  sev.sigev_notify_function = info_bar_finish;
  timer_create(CLOCK_REALTIME, &sev, &t_info_bar_id);

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

int ctrl_set_model(model_t *model)
{
	_model = model;
	return 0;
}
