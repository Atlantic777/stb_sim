#ifndef ZAPPER_FSM_H
#define ZAPPER_FSM_H

#include <time.h>
#include <signal.h>

static const int WAIT_INTERVAL = 3;

typedef void (*State)(char);
typedef void(*info_callback_t)(int chan);

typedef enum zapp_fsm_signal {
  SIG_NOOP,
  SIG_KEY_DOWN,
  SIG_KEY_TIMEOUT,
  SIG_MAX
} zapp_fsm_signal_t;

// timer
static struct sigevent sev;
static timer_t timer_id;
static struct itimerspec fTime;
static struct itimerspec oTime;

extern State state;
extern char input_len;
extern char input_buff[4];
extern int stopped;
extern int current_chan;
extern info_callback_t info_callback;

// state handlers
void state_idle(char ev);
void state_num_enter(char ev);
void state_stop(char ev);
void state_update(char ev);

// private util functions
void schedule_update();
int isNum(char ev);
void process_buffer();
void store_num(char ev);

// public interface
int  zapp_fsm_init();
int  zapp_fsm_deinit();
int  zapp_fsm_stopped();
int  zapp_fsm_get_current_ch();

void zapp_fsm_key_press(char ev);


#endif
