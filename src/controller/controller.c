/**
 * @file controller.c
 * @author Nikola Hardi
 * @date November 2015
 * @brief Implementation of STB controller part of MVC
 */
// TODO. handle info key (show info bar)
// TODO: add zap finish handler
// TODO: make a difference when num input timeout
// TODO: check if vol change works while zapping
// TODO: check multiple ch change events
// TODO: don't forge default config parser
// TODO: proper controller thread join (main f structure)
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "controller.h"
#include "model.h"
#include "controller/norm_codes.h"

static view_t   *_view;
static model_t  *_model;
State _state;

static void state_scan(signal_t, uint32_t);
static void state_idle(signal_t, uint32_t);
static void state_num_enter(signal_t, uint32_t);

static char nums[4];
static uint8_t  nums_len;
static uint8_t  current_chan = 1;

static struct itimerspec its;
static struct sigevent sev;

static timer_t t_num_input_id;
static timer_t t_info_bar_id;
static timer_t t_volume_id;

static void next_chan();
static void prev_chan();
static void vol_up();
static void vol_down();

static void next_chan()
{
	_model->ch_up();
  // TODO:
  // R: view accepts ch description
  // get current ch description
  // show view with current ch
}

static void prev_chan()
{
	_model->ch_down();
}

static void vol_up()
{
	_model->vol_up();
}

static void vol_down()
{
	_model->vol_down();
  // TODO: view volume handler
	_view->show_volume(_model->get_volume());
}

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

  // TODO:
  // if timeout -> switch to ch by ch by order
  // if 3-key-input -> switch to ch by ch number

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

	current_chan = n;

  timer_settime(t_num_input_id, 0, &its, NULL);
  _view->show_num_input(nums);
	_model->ch_switch(n);
}

// ############ STATES ################
static void state_scan(signal_t sig, uint32_t args)
{
  LOG("scanning...");
  _state = state_idle;
  _state(SIG_NOOP, 0);

  LOG("scan state finished");
}

static void state_idle(signal_t sig, uint32_t args)
{
	LOG("idle state handler got key");
  if(SIG_NUM_KEY == sig)
  {
    store_num(args);
    _state = state_num_enter;
  }
	if(SIG_CH_CHANGE == sig)
	{
		if(N_KEY_CH_UP == args)
		{
			next_chan();
		}
		else if(N_KEY_CH_DN == args)
		{
			prev_chan();
		}
	}
	if(SIG_VOL_CHANGE == sig)
	{
		if(N_KEY_VOL_UP == args)
		{
			vol_up();
		}
		else if(N_KEY_VOL_DN == args)
		{
			vol_down();
		}
	}
  LOG("Idle state...");
}

static void state_num_enter(signal_t sig, uint32_t args)
{
  LOG("num enter");
  if(SIG_NUM_KEY == sig)
  {
    store_num(args);

    // TODO: if 3 keys -> go to finish nums with 3-key-signal
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
  // TODO: pass A/V stream types from init config
	_model->init();
}

static void parse_config()
{
  LOG("Parse config...");
  // TODO
  /* init_config_parse() */
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

  // TODO: pass config file path
  parse_config();

  switch_default();

  _state(SIG_NOOP, 0);
}

static void stop()
{
}

static uint8_t isNum(uint32_t c)
{
  if(c >= 0 && c <= 9)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

static signal_t decode_signal(uint32_t ev)
{
  if(isNum(ev))
  {
    LOG("SIG_NUM_KEY");
    return SIG_NUM_KEY;
  }
  else if(N_KEY_EXIT == ev)
  {
    LOG("SIG_KILL");
    return SIG_KILL;
  }
	else if(N_KEY_CH_UP == ev || N_KEY_CH_DN == ev)
	{
		return SIG_CH_CHANGE;
	}
	else if(N_KEY_VOL_UP == ev || N_KEY_VOL_DN == ev)
	{
		return SIG_VOL_CHANGE;
	}
}

static void input_handler(uint32_t ev)
{
  signal_t sig = decode_signal(ev);

  if(SIG_KILL == sig)
  {
		_model->deinit();
    exit(0);
  }

  _state(sig, ev);
}

/// \brief Nothing to see here
uint8_t ctrl_init(controller_t *ctrl)
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

  // TODO add timeout args
  sev.sigev_notify_function = num_finish;
  timer_create(CLOCK_REALTIME, &sev, &t_num_input_id);

  sev.sigev_notify_function = info_bar_finish;
  timer_create(CLOCK_REALTIME, &sev, &t_info_bar_id);

  // TODO: volume autohide
  /* sev.sigev_notify_function = volume_info_finish; */
  /* timer_create(CLOCK_REALTIME, &sev, &t_info_bar_id); */

  // set location for default config

  return 0;
}

uint8_t ctrl_deinit()
{
  return 1;
}

/**
 * @brief Sets active view
 *
 * @param view pointer
 *
 * @return  status code
 */
uint8_t ctrl_set_view(view_t *view)
{
  _view = view;
  return 0;
}

uint8_t ctrl_set_model(model_t *model)
{
	_model = model;
	return 0;
}
